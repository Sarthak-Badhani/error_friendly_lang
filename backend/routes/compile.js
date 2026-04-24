/**
 * routes/compile.js — Compilation Route
 *
 * POST /compile
 *
 * 1. Receives user code from the request body
 * 2. Saves it as a temporary .ef file
 * 3. Runs compiler.exe on the file using child_process
 * 4. Captures stdout/stderr and returns the result
 * 5. Cleans up the temp file afterwards
 */

const express = require('express');
const router = express.Router();
const { execFile } = require('child_process');
const fs = require('fs').promises;
const path = require('path');

// ── Paths ──
// Absolute path to the compiler executable
const COMPILER_PATH = 'x:\\CD\\error_friendly_lang\\bin\\compiler.exe';
// Directory to store temporary source files
const TEMP_DIR = path.join(__dirname, '..', 'temp');

// Timeout for compiler execution (10 seconds)
const COMPILE_TIMEOUT = 10000;

// Simple lock to prevent overlapping rapid requests
let isCompiling = false;

/**
 * POST /compile
 * Body: { "code": "string" }
 * Returns: { "success": bool, "output": "string" } or { "success": false, "error": "string" }
 */
router.post('/', async (req, res) => {
  const { code } = req.body;

  // ── Validate request ──
  if (!code || typeof code !== 'string') {
    return res.status(400).json({
      success: false,
      error: 'No code provided. Send { "code": "your code here" } in the request body.',
    });
  }

  // ── Prevent rapid-fire requests ──
  if (isCompiling) {
    return res.status(429).json({
      success: false,
      error: 'A compilation is already in progress. Please wait.',
    });
  }

  isCompiling = true;

  // Generate a unique filename to avoid collisions
  const filename = `program_${Date.now()}.ef`;
  const tempFilePath = path.join(TEMP_DIR, filename);

  try {
    // ── Step 1: Ensure temp directory exists ──
    await fs.mkdir(TEMP_DIR, { recursive: true });

    // ── Step 2: Write code to a temporary .ef file ──
    await fs.writeFile(tempFilePath, code, 'utf-8');

    // ── Step 3: Run the compiler on the temp file ──
    const result = await runCompiler(tempFilePath);

    // ── Step 4: Return the result ──
    // The compiler prints all output to stdout (both success and errors).
    // We determine success by checking if "COMPILATION SUCCESSFUL" appears.
    const isSuccess = result.output.includes('COMPILATION SUCCESSFUL');

    return res.json({
      success: isSuccess,
      output: result.output,
      ...(result.stderr ? { error: result.stderr } : {}),
    });

  } catch (error) {
    // Unexpected error (e.g., compiler not found, timeout)
    return res.json({
      success: false,
      error: error.message || 'An unknown error occurred during compilation.',
    });

  } finally {
    // ── Step 5: Clean up the temp file ──
    isCompiling = false;
    try {
      await fs.unlink(tempFilePath);
    } catch {
      // Silently ignore cleanup errors (file may not exist)
    }
  }
});

/**
 * runCompiler — Executes the C++ compiler on a given source file.
 *
 * Uses execFile for safer execution on Windows.
 * Always resolves with { output, stderr, exitCode } since the compiler
 * returns exit code 1 for programs with errors but still prints
 * useful diagnostic output to stdout.
 *
 * @param {string} filePath - Absolute path to the .ef source file
 * @returns {Promise<{output: string, stderr: string, exitCode: number}>}
 */
function runCompiler(filePath) {
  return new Promise((resolve, reject) => {
    execFile(
      COMPILER_PATH,
      [filePath],
      { timeout: COMPILE_TIMEOUT, windowsHide: true },
      (error, stdout, stderr) => {
        // If the compiler timed out
        if (error && error.killed) {
          return reject(
            new Error('Compilation timed out (exceeded 10 seconds). Your program may contain an infinite loop.')
          );
        }

        // If the compiler executable was not found
        if (error && error.code === 'ENOENT') {
          return reject(
            new Error(`Compiler not found at: ${COMPILER_PATH}`)
          );
        }

        // For all other cases, collect whatever output we have.
        // The compiler may exit with code 1 (errors found) but stdout
        // still contains all the diagnostic output we want to show.
        const combinedOutput = (stdout || '') + (stderr || '');

        if (!combinedOutput.trim()) {
          // No output at all — something unexpected happened
          if (error) {
            return reject(new Error(`Compiler exited with code ${error.code}: ${error.message}`));
          }
          return resolve({ output: 'Compilation completed with no output.', stderr: '', exitCode: 0 });
        }

        resolve({
          output: combinedOutput.trim(),
          stderr: (stderr || '').trim(),
          exitCode: error ? error.code : 0,
        });
      }
    );
  });
}

module.exports = router;

