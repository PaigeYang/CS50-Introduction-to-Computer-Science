-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See decraiptiobs in the crime day in Humphrey Street
SELECT description FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND Street = "Humphrey Street";
-- ANS: Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- ANS: Littering took place at 16:36. No known witnesses.

-- Check who are interviewers
SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";
-- Ruth: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene: I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond: As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Check atm_transactions and bank accounts
SELECT name, phone_number, passport_number, license_plate FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- suspects:Bruce, Diana, Brooke, Kenny, Iman, Luca, Taylor, Benista

-- Match phone calls ans suspects less than a minute
SELECT hour, minute, activity, people.license_plate, name, caller, receiver, duration
FROM bakery_security_logs JOIN people ON bakery_security_logs.license_plate = people.license_plate
JOIN phone_calls ON phone_number = phone_calls.caller
WHERE bakery_security_logs.year = 2021 AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28
AND bakery_security_logs.hour between 10 AND 11 AND activity="exit"
AND duration < 60;
-- suspects narrow down: Bruce, Diana, Taylor

-- Check bakery security logs, and connect with people's table
SELECT hour, minute, activity, people.license_plate, name, phone_number
FROM bakery_security_logs JOIN people ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2021 AND month = 7 AND day = 28 AND hour between 10 AND 11 AND activity="exit" AND name IN ("Bruce", "Diana");
-- suspects narrow down: Bruce, Diana, (Taylor超過十分鐘故排除)


--Check flight number where orginal airport id = 8 ()
SELECT id, abbreviation, full_name, city
FROM airports;

-- Check flights and destination city on July 29, 2021
SELECT flights.id, day, hour, minute, city AS destination
FROM flights
JOIN airports ON destination_airport_id = airports.id
WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour;
--  ANS. The earliest flight out of Fiftyville is New York City

-- Check Who is in the flight on July 29, 2021
SELECT name
FROM passengers
JOIN flights ON flight_id = flights.id
JOIN people ON passengers.passport_number = people.passport_number
WHERE flight_id = 36 AND name IN ("Bruce", "Diana");
--  ANS. Bruce



-- Check who was Bruce calling for?
SELECT *
FROM people
WHERE name="Bruce";

SELECT *
FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28
AND duration < 60
AND caller = "(367) 555-5533";

-- Check receiver's phone number
SELECT *
FROM people
Where phone_number IN ("(375) 555-8161");

-- THe accomplice is Robin






