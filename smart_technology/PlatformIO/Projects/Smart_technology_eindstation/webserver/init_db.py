import sqlite3

connection = sqlite3.connect('database.db')


with open('schema.sql') as f:
    connection.executescript(f.read())

cur = connection.cursor()

cur.execute("INSERT INTO weatherdata (ip, temperature) VALUES (?, ?)",
            ('192.168.43.83', 18)
            )

connection.commit()
connection.close()