import cs50
from sys import argv, exit


def main():

    # open the file for SQLite
    db = cs50.SQL("sqlite:///students.db")

    # check if the command-line argumet is 2
    if len(argv) != 2:
        print("Usage: python roster.py house")
        exit(1)

    # Fetch all data from user's input
    value = db.execute("SELECT * FROM students WHERE house = ? ORDER by last, first", argv[1])

    for row in value:

        # skip middle name if None
        if row['middle'] == None:
            print(f"{row['first']} {row['last']}, born {row['birth']}")
        else:
            print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")


main()