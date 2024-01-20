from cs50 import get_int

creditnum = get_int("Number: ")
x = creditnum

# create a dictionary to store every digit of the credit number
credit = {}

creditdigits = 0

# create a loop to stroe all digits
while True:
    # the remainder of the last digit of credit number
    y = x % 10

    # calculate the digit from the last number
    creditdigits = creditdigits + 1

    # store the digits in dictionary
    credit[creditdigits] = y

    # subtract 1 from creditdigits
    x = int(x / 10)

    if x == 0:
        break

sum = 0


# calculate even digit
for i in range(2, len(credit)+1, 2):

    new = credit[i] * 2

    if new > 9:
        sum = sum + 1 + new % 10
    else:
        sum = sum + new

# Add odd digit
for i in range(1, len(credit)+1, 2):
    sum = sum + credit[i]


if sum % 10 != 0:
    print("INVALID")

else:
    # check the first two digits of the credit number
    digits = int(str(creditnum)[0:2])

    if creditdigits == 15:
        if digits == 34 or digits == 37:
            print("AMEX")
        else:
            print("INVALID")

    elif creditdigits == 16:
        if digits > 39 and digits < 50:
            print("VISA")

        elif digits > 50 and digits < 56:
            print("MASTERCARD")

        else:
            print("INVALID")

    elif creditdigits == 13:
        if digits > 39 and digits < 50:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")
