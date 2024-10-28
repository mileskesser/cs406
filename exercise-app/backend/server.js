const express = require('express');
const cors = require('cors');
const path = require('path');
const { v4: uuidv4 } = require('uuid');

const app = express();
const port = 5002;

app.use(cors());
app.use(express.json());

app.use(express.static(path.join(__dirname, '../frontend')));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '../frontend', 'index.html'));
});

let exercises = [];

// API to get all exercises
app.get('/api/exercises', (req, res) => {
  res.json(exercises);
});

// API to get a single exercise by ID
app.get('/api/exercises/:id', (req, res) => {
  const { id } = req.params;
  const exercise = exercises.find(ex => ex.id === id);
  if (exercise) {
    res.json(exercise);
  } else {
    res.status(404).json({ message: 'Exercise not found' });
  }
});

// API to add a new exercise
app.post('/api/exercises', (req, res) => {
  const { id, name, reps, weight, unit, date } = req.body;
  const newExercise = { id: id || uuidv4(), name, reps, weight, unit, date }; // Add exercise with UUID if no ID exists
  exercises.push(newExercise);
  res.json({ message: 'Exercise added successfully', exercise: newExercise });
});

// API to delete an exercise by ID
app.delete('/api/exercises/:id', (req, res) => {
  const { id } = req.params;
  exercises = exercises.filter(exercise => exercise.id !== id);
  res.json({ message: 'Exercise deleted successfully' });
});

// API to edit an exercise by ID
app.put('/api/exercises/:id', (req, res) => {
  const { id } = req.params;
  const { name, reps, weight, unit, date } = req.body;

  let exerciseFound = false;
  
  exercises = exercises.map(exercise => {
    if (exercise.id === id) {
      exerciseFound = true;
      return { id, name, reps, weight, unit, date };
    }
    return exercise;
  });

  if (exerciseFound) {
    res.json({ message: 'Exercise updated successfully' });
  } else {
    res.status(404).json({ message: 'Exercise not found' });
  }
});

// Start the server
app.listen(port, () => {
  console.log(`Server running on http://localhost:${port}`);
});
