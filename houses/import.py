import cs50
from sys import argv, exit
from csv import reader, DictReader


def main():

    # open the file for SQLite
    db = cs50.SQL("sqlite:///students.db")

    # Create table called students in database file called studnes.db
    # db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

    # check if the command-line argumet is 2
    if len(argv) != 2:
        print("Usage: python import.py characters.csv")
        exit(1)

    # open the csv file
    with open(argv[1], newline='') as csvfile:

        file = DictReader(csvfile)

        for row in file:
            name = row['name']
            splitname = name.split()

            if len(splitname) == 3:
                firstname = splitname[0]
                middlename = splitname[1]
                lastname = splitname[2]
            else:
                firstname = splitname[0]
                lastname = splitname[1]
                middlename = None

            # Insert the data from csv file to SQLite database
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       firstname, middlename, lastname, row['house'], row['birth'])


main()