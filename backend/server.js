/**
 * server.js — Express Backend for Error-Friendly Language Compiler
 *
 * Starts an Express server on port 5000 that:
 *  - Accepts code from the React frontend
 *  - Runs the C++ compiler executable on it
 *  - Returns compiler output/errors back to the frontend
 */

const express = require('express');
const cors = require('cors');
const compileRoute = require('./routes/compile');

const app = express();
const PORT = 5000;

// ── Middleware ──
app.use(cors());                         // Allow cross-origin requests from frontend
app.use(express.json({ limit: '1mb' })); // Parse JSON request bodies

// ── Routes ──
app.use('/compile', compileRoute);

// ── Health check endpoint ──
app.get('/', (req, res) => {
  res.json({
    status: 'ok',
    message: 'Error-Friendly Language Compiler Backend',
    version: '1.0.0',
  });
});

// ── Start server ──
app.listen(PORT, () => {
  console.log(`\n  ⚡ Compiler backend running at http://localhost:${PORT}`);
  console.log(`  📡 Accepting requests on POST /compile\n`);
});
