from sys import argv, exit
from csv import reader, DictReader
import re


def main():

    # check if users type in with the incorrect number of command-line arguments
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # open the text file
    f = open(argv[2], 'r')

    # save letters from text file to variable
    text = f.read()

    # the check value from text file
    check = []

    # open the csv file
    with open(argv[1], newline='') as csvfile:

        # read the file
        rows = reader(csvfile)

        # read the first line of the file
        first_line = next(rows)

        columns = len(first_line)
        #print (columns)

        # get STR from the file
        for i in first_line[1:]:
            out = re.findall(rf'(?:{i})+', text)

            # if there is no out, it's No match
            if out == []:
                print('No match')
                exit(2)

            largest = max(out, key=len)
            num = len(largest)/len(i)

            # append to the check list
            check.append(num)

        # put the file data in the list
        file = []
        for row in rows:
            file.append(row)

        # check if STRs in the text file match to the person in the csv
        for i in range(len(file)):
            count = 0
            for j in range(len(check)):

                if (int(file[i][j+1]) == check[j]):
                    count = count + 1
                else:
                    break

                if count == len(check):
                    print(file[i][0])
                    return 0

        print('No match')

    # close the text file
    f.close()


main()
