from flask import Flask, render_template, request, redirect, url_for
import sqlite3

app = Flask(__name__)
DATABASE = 'database_visualizer.db'

def get_db_connection():
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row
    return conn

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/view_tables')
def view_tables():
    conn = get_db_connection()
    cursor = conn.cursor()
    # Exclude 'sqlite_sequence' from the list of tables
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';")
    tables = [row['name'] for row in cursor.fetchall()]
    conn.close()
    return render_template('view_tables.html', tables=tables)

@app.route('/view_table/<table_name>')
def view_table(table_name):
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute(f"PRAGMA table_info({table_name})")
    schema = cursor.fetchall()
    cursor.execute(f"SELECT * FROM {table_name}")
    data = cursor.fetchall()
    conn.close()
    return render_template('view_table.html', table_name=table_name, schema=schema, data=data)

@app.route('/execute_query', methods=['POST'])
def execute_query():
    query = request.form['query']
    conn = get_db_connection()
    cursor = conn.cursor()
    try:
        cursor.execute(query)
        if query.strip().lower().startswith("select"):
            result = cursor.fetchall()
            conn.close()
            return render_template('index.html', message="Query executed successfully!", success=True, query=query, result=result)
        else:
            conn.commit()
            conn.close()
            return render_template('index.html', message="Query executed successfully!", success=True)
    except Exception as e:
        conn.close()
        return render_template('index.html', message=f"Query failed. Error: {str(e)}", success=False)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5009, debug=True)
