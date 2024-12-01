const express = require('express');
const sqlite3 = require('sqlite3').verbose();
const path = require('path');
const bodyParser = require('body-parser');

const app = express();
const DATABASE = 'database_visualizer.db';
const PORT = 5009;

// Set up middleware
app.use(express.static(path.join(__dirname, 'public')));

app.use(bodyParser.urlencoded({ extended: true }));

// Connect to SQLite database
function getDbConnection() {
  const db = new sqlite3.Database(DATABASE, (err) => {
    if (err) {
      console.error('Error connecting to the database:', err.message);
    }
  });
  return db;
}

// Route: Home (index.html)
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'templates', 'index.html'));
});

// Route: View tables
app.get('/view_tables', (req, res) => {
  const db = getDbConnection();
  const query = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";

  db.all(query, [], (err, tables) => {
    if (err) {
      console.error('Error fetching tables:', err.message);
      res.send('Error fetching tables');
    } else {
      res.render(path.join(__dirname, 'templates', 'view_tables.html'), { tables });
    }
    db.close();
  });
});

// Route: View specific table
app.get('/view_table/:table_name', (req, res) => {
  const tableName = req.params.table_name;
  const db = getDbConnection();

  const schemaQuery = `PRAGMA table_info(${tableName});`;
  const dataQuery = `SELECT * FROM ${tableName};`;

  db.all(schemaQuery, [], (err, schema) => {
    if (err) {
      console.error(`Error fetching schema for ${tableName}:`, err.message);
      res.send(`Error fetching schema for ${tableName}`);
    } else {
      db.all(dataQuery, [], (err, data) => {
        if (err) {
          console.error(`Error fetching data for ${tableName}:`, err.message);
          res.send(`Error fetching data for ${tableName}`);
        } else {
          res.render(path.join(__dirname, 'templates', 'view_table.html'), { tableName, schema, data });
        }
        db.close();
      });
    }
  });
});

// Route: Execute SQL query
app.post('/execute_query', (req, res) => {
  const query = req.body.query;
  const db = getDbConnection();

  db.all(query, [], (err, rows) => {
    if (err) {
      console.error('Error executing query:', err.message);
      res.render(path.join(__dirname, 'templates', 'index.html'), { query, result: `Error: ${err.message}` });
    } else {
      res.render(path.join(__dirname, 'templates', 'index.html'), { query, result: rows });
    }
    db.close();
  });
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
