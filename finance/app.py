import os
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    rows = db.execute('''SELECT symbol, name, SUM(shares) FROM "index"
                    WHERE user_id = :user_id
                    GROUP BY symbol
                    HAVING SUM(shares) > 0
                    ORDER BY symbol''' , user_id=session["user_id"])


    cash = db.execute('''SELECT cash FROM users
                    WHERE id = :user_id''' , user_id=session["user_id"])

    total_value=round(cash[0]['cash'],2)



    #Add current price & value in the rows
    for i in range(len(rows)):
        rows[i]['current_price']=round(lookup(rows[i]['symbol'])['price'],2)
        rows[i]['current_value']=round(lookup(rows[i]['symbol'])['price'],2) * rows[i]['SUM(shares)']
        total_value = total_value + rows[i]['current_value']
        print (rows[i]['current_value'])


    return render_template('index.html', rows=rows, total_value=total_value, cash=cash)




@app.route("/sellout", methods=["POST"])
@login_required
def sellout():
    rows = db.execute('''SELECT symbol, name, SUM(shares) FROM "index"
                           WHERE user_id = :user_id
                           GROUP BY symbol
                           HAVING SUM(shares) > 0
                           ORDER BY symbol''' , user_id=session["user_id"])
    money = 0
    for i in range(len(rows)):
            sell_num = request.form.get(rows[i]['symbol'])

            if request.form.get(rows[i]['symbol']) != "Select":

                stock = lookup(rows[i]['symbol'])
                money = money + round(float(stock['price']),2)*int(sell_num)

    # get the blance of user
    blance = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])

    #calculate after user sold it
    left = blance[0]['cash'] + money

    for i in range(len(rows)):

        #update the data if you select
        if request.form.get(rows[i]['symbol']) != "Select":
            #update the new blance
            db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=left, user_id=session["user_id"])

            #Get the datetime
            time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            #record in table index
            db.execute('''INSERT INTO 'index' (user_id, symbol, name, shares, price, total, transacted)
            VALUES (:user_id, :symbol, :name, :shares, :price, :total, :transacted)''',
                    user_id=session["user_id"],
                    symbol=rows[i]['symbol'],
                    name=rows[i]['name'],
                    shares=int(request.form.get(rows[i]['symbol']))*-1,
                    price=lookup(rows[i]['symbol'])['price'],
                    total=round(float(stock['price']),2)*-1*int(lookup(rows[i]['symbol'])['price']),
                    transacted=time)
    return redirect("/")

@app.route("/buyin", methods=["POST"])
@login_required
def buyin():
    rows = db.execute('''SELECT symbol, name, SUM(shares) FROM "index"
                           WHERE user_id = :user_id
                           GROUP BY symbol
                           HAVING SUM(shares) > 0
                           ORDER BY symbol''' , user_id=session["user_id"])
    spend = 0
    for i in range(len(rows)):
            buy_num = request.form.get(rows[i]['symbol'])

            if request.form.get(rows[i]['symbol']) != "Select":

                stock = lookup(rows[i]['symbol'])
                spend = spend + round(float(stock['price']),2)*int(buy_num)

    # get the blance of user
    blance = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])

    #calculate after user bough it
    left = blance[0]['cash'] - spend

    # check if user have enough money
    if left < 0 :
        return apology("You don't have enough moeny to buy", 400)
    if left != int(left) :
        return apology("You must type integral", 400)

    else:
        for i in range(len(rows)):

            #update the data if you select
            if request.form.get(rows[i]['symbol']) != "Select":
                #update the new blance
                db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=left, user_id=session["user_id"])

                #Get the datetime
                time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

                #record in table index
                db.execute('''INSERT INTO 'index' (user_id, symbol, name, shares, price, total, transacted)
                VALUES (:user_id, :symbol, :name, :shares, :price, :total, :transacted)''',
                          user_id=session["user_id"],
                          symbol=rows[i]['symbol'],
                          name=rows[i]['name'],
                          shares=request.form.get(rows[i]['symbol']),
                          price=lookup(rows[i]['symbol'])['price'],
                          total=round(float(stock['price']),2)*int(lookup(rows[i]['symbol'])['price']),
                          transacted=time)
    return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == 'POST':

        # get the blance of user
        blance = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])


        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide the stock you want to buy", 400)

        # Ensure share was submitted
        if not request.form.get("shares"):
            return apology("must provide the number of share you want to buy", 400)

        #Check if the shares are positive integer
        if int(request.form.get("shares")) < 0:
            return apology("the number of sahre you want to buy must be a positive integer", 400)


        # get the stock user wants to buy
        stock = lookup(request.form.get("symbol"))
        share = request.form.get('shares')

        # Ensure the stock exists
        if stock == None:
            return apology("Plese fill in correct stock symbol", 400)

        #calculate after user bough it
        left = blance[0]['cash'] - round(float(stock['price']),2)*int(share)

        # check if user have enough money
        if left < 0:
            return apology("You don't have enough moeny to buy", 403)
        else:
            #update the new blance
            db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=left, user_id=session["user_id"])

            #Get the datetime
            time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            #record in table index
            db.execute('''INSERT INTO 'index' (user_id, symbol, name, shares, price, total, transacted)
            VALUES (:user_id, :symbol, :name, :shares, :price, :total, :transacted)''',
                       user_id=session["user_id"],
                       symbol=stock['symbol'],
                       name=stock['name'],
                       shares=share,
                       price=stock['price'],
                       total=round(float(stock['price']),2)*int(share),
                       transacted=time)

            return redirect('/')

    else:
        return render_template('buy.html')

@app.route("/history")
@login_required
def history():

    rows = db.execute('''SELECT symbol, name, shares, price, transacted FROM "index"
                           WHERE user_id = :user_id
                           ORDER BY transacted DESC''' , user_id=session["user_id"])

    return render_template('history.html', rows=rows)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        print (rows)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))

        if not stock:
            return apology("stock error", 400)

        else:
            return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    # Forget any user_id
    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username is single
        if len(rows) != 0:
            return apology("the username already exist, please change username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password and password (again) are the same
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Please make sure the password and password (again) are the same", 400)

        # Store the new users in the database
        hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=hash)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    #list all share the user has
    rows = db.execute('''SELECT symbol, SUM(shares) FROM users
                    JOIN 'index' ON id = user_id WHERE id = :user_id
                    GROUP BY symbol
                    Having SUM(shares) > 0
                    ORDER BY symbol''' , user_id=session["user_id"])

    if request.method == "POST":

        # get the blance of user
        blance = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])


        # Ensure symbol was selected
        if request.form.get("symbol") == "default":
            return apology("plese choose the symbol", 400)

        # Ensure share was submitted
        if not request.form.get("shares"):
            return apology("must provide the number of share you want to sell", 400)

        #Check if the shares are positive integer
        if int(request.form.get("shares")) < 0:
            return apology("the number of sahre you want to sell must be a positive integer", 400)


        # get the stock user wants to sell
        stock = lookup(request.form.get("symbol"))
        share = request.form.get('shares')


        #selected symbol
        selected = db.execute('''SELECT symbol, SUM(shares) FROM users
                           JOIN 'index' ON id = user_id WHERE id = :user_id
                           GROUP BY symbol
                           HAVING symbol = :symbol''' ,
                           user_id=session["user_id"], symbol=stock['symbol'])

        #Alert if the user doesn't have enough shares
        if selected[0]["SUM(shares)"] < int(share):
            return apology("You don't have enough shars to sell", 400)
        else:
            cash = blance[0]['cash'] + round(float(stock['price']),2)*int(share)

            #update the new blance
            db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cash, user_id=session["user_id"])

            #Get the datetime
            time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            #record in table index
            db.execute('''INSERT INTO 'index' (user_id, symbol, name, shares, price, total, transacted)
            VALUES (:user_id, :symbol, :name, :shares, :price, :total, :transacted)''',
                       user_id=session["user_id"],
                       symbol=stock['symbol'],
                       name=stock['name'],
                       shares=-1*int(share),
                       price=stock['price'],
                       total=round(float(stock['price']),2)*-1*int(share),
                       transacted=time)

            return redirect("/")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", rows=rows)

@app.route("/password", methods=["GET", "POST"])
@login_required
def password():

    if request.method == "POST":

        # Ensure the passowrd was submitted
        if not request.form.get("old-password"):
            return apology("must provide your password", 403)

        hash = db.execute("SELECT hash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])

        # Ensure password is correct
        if not check_password_hash(hash[0]["hash"], request.form.get("old-password")):
            return apology("invalid password", 403)

        # Ensure the new passowrd was submitted
        if not request.form.get("new-password"):
            return apology("must provide your new password", 403)

        # Ensure the new passowrd (confirmation) was submitted
        if not request.form.get("confirmation"):
            return apology("must provide your password (again)", 403)

        if request.form.get("new-password") != request.form.get("confirmation"):
            return apology("Please make sure your new password and new password (again) are the same", 403)

        # Store the new password in the database
        new_hash = generate_password_hash(request.form.get("new-password"), method='pbkdf2:sha256', salt_length=8)
        db.execute("UPDATE users SET hash = :new_hash WHERE id = :user_id", new_hash=new_hash, user_id=session["user_id"])

        message = "Password updated"
        return render_template("password.html", message=message)
    else:
        return render_template("password.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
