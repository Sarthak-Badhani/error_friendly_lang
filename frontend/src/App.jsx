/**
 * App.jsx — Main Application Component
 *
 * Orchestrates the compiler UI with:
 *  - A top bar: title left, Run center, About right
 *  - A split-screen layout (editor left, output right)
 *  - Keyboard shortcut support (Ctrl+Enter to run)
 *  - Mock compilation logic (ready for backend integration)
 */

import React, { useState, useEffect, useCallback } from 'react';
import Editor, { DEFAULT_CODE } from './components/Editor';
import Output from './components/Output';
import About from './components/About';
import './App.css';

function App() {
  // ── State ──
  const [code, setCode] = useState(DEFAULT_CODE);
  const [output, setOutput] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const [showAbout, setShowAbout] = useState(false);

  // Backend API endpoint
  const API_URL = 'http://localhost:5000/compile';

  /**
   * handleRun — Sends code to the backend compiler and displays the result.
   *
   * Makes a POST request to the Express backend which:
   *  1. Saves the code as a .ef temp file
   *  2. Runs compiler.exe on it
   *  3. Returns stdout/stderr
   */
  const handleRun = useCallback(async () => {
    // Prevent double-clicks while compiling
    if (isLoading) return;

    setIsLoading(true);
    setOutput('');

    try {
      const response = await fetch(API_URL, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ code }),
      });

      const data = await response.json();

      if (data.output) {
        // Show the full compiler output (works for both success and errors)
        setOutput(data.output);
      } else if (data.error) {
        // Fallback: show error message if no output
        setOutput(data.error);
      } else {
        setOutput('No output received from compiler.');
      }
    } catch (error) {
      // Network error or backend is down
      setOutput(
        '❌ CONNECTION ERROR\n\n' +
        'Could not reach the compiler backend.\n' +
        'Make sure the backend server is running:\n\n' +
        '  cd backend\n' +
        '  node server.js\n\n' +
        `Details: ${error.message}`
      );
    } finally {
      setIsLoading(false);
    }
  }, [code, isLoading]);

  /**
   * Clear all output from the terminal panel.
   */
  const handleClear = useCallback(() => {
    setOutput('');
  }, []);

  /**
   * Global keyboard shortcut: Ctrl+Enter to compile.
   * Registered once on mount, cleaned up on unmount.
   */
  useEffect(() => {
    const handleKeyDown = (e) => {
      if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
        e.preventDefault();
        handleRun();
      }
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [handleRun]);

  return (
    <div className="app">
      {/* ── Top Bar — 3-section layout ── */}
      <header className="topbar">
        {/* Left: Title */}
        <div className="topbar-left">
          <h1 className="topbar-title">Error Friendly Language</h1>
        </div>

        {/* Center: Run Button */}
        <div className="topbar-center">
          <button
            id="run-button"
            className={`run-btn ${isLoading ? 'loading' : ''}`}
            onClick={handleRun}
            disabled={isLoading}
            aria-label="Run compiler"
          >
            {isLoading ? (
              <>
                <span className="btn-spinner" />
                Compiling
              </>
            ) : (
              <>
                <svg className="play-icon" width="16" height="16" viewBox="0 0 24 24" fill="currentColor">
                  <polygon points="5 3 19 12 5 21 5 3" />
                </svg>
                Run
              </>
            )}
          </button>
        </div>

        {/* Right: About Button */}
        <div className="topbar-right">
          <button
            id="about-button"
            className="about-btn"
            onClick={() => setShowAbout(true)}
            aria-label="About this language"
          >
            <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
              <circle cx="12" cy="12" r="10" />
              <line x1="12" y1="16" x2="12" y2="12" />
              <line x1="12" y1="8" x2="12.01" y2="8" />
            </svg>
            About
          </button>
        </div>
      </header>

      {/* ── Main Content: Split-screen Layout ── */}
      <main className="workspace">
        {/* Left panel — Code Editor (50%) */}
        <section className="panel editor-panel" aria-label="Code Editor">
          <Editor code={code} onChange={setCode} />
        </section>

        {/* Right panel — Output Terminal (50%) */}
        <section className="panel output-panel" aria-label="Compiler Output">
          <Output
            output={output}
            isLoading={isLoading}
            onClear={handleClear}
          />
        </section>
      </main>

      {/* ── About Modal ── */}
      <About isOpen={showAbout} onClose={() => setShowAbout(false)} />
    </div>
  );
}

export default App;
