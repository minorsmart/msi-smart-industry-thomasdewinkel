DROP TABLE IF EXISTS weatherdata;

CREATE TABLE weatherdata (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    ip TEXT NOT NULL,
    temperature INTEGER NOT NULL
    
);