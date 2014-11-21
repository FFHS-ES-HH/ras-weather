CREATE TABLE piw_values IF NOT EXISTS (
    id INTEGER PRIMARY KEY ASC,
    temperature REAL NOT NULL,
    humidity REAL NOT NULL,
    pressure REAL NOT NULL,
    illuminance REAL NOT NULL,
    created_at INTEGER NOT NULL);

