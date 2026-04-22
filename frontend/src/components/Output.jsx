/**
 * Output.jsx — Terminal-style Output Panel Component
 *
 * Displays compiler output in a styled terminal-like panel.
 * Shows green text for success, red for errors, and includes
 * a clear button and auto-scroll behavior.
 */

import React, { useEffect, useRef } from 'react';
import './Output.css';

function Output({ output, isLoading, onClear }) {
  const outputRef = useRef(null);

  /**
   * Auto-scroll to the bottom whenever new output arrives
   * so the user always sees the latest result.
   */
  useEffect(() => {
    if (outputRef.current) {
      outputRef.current.scrollTop = outputRef.current.scrollHeight;
    }
  }, [output]);

  return (
    <div className="output-container">
      {/* Output panel header */}
      <div className="output-header">
        <div className="output-header-left">
          <span className="output-icon">⬛</span>
          <span className="output-title">Terminal Output</span>
          {isLoading && (
            <span className="output-status compiling">
              <span className="status-dot pulse" />
              Compiling...
            </span>
          )}
          {!isLoading && output && (
            <span className={`output-status ${output.includes('ERROR') || output.includes('❌') ? 'error' : 'success'}`}>
              <span className="status-dot" />
              {output.includes('ERROR') || output.includes('❌') ? 'Errors found' : 'Ready'}
            </span>
          )}
        </div>

        {/* Clear output button */}
        <button
          className="clear-btn"
          onClick={onClear}
          title="Clear output"
          aria-label="Clear terminal output"
          disabled={!output && !isLoading}
        >
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
            <polyline points="3 6 5 6 21 6" />
            <path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2" />
          </svg>
          Clear
        </button>
      </div>

      {/* Terminal output body */}
      <div className="output-body" ref={outputRef}>
        {isLoading ? (
          <div className="output-loading">
            <div className="loading-bar">
              <div className="loading-bar-fill" />
            </div>
            <pre className="output-text loading-text">
              <span className="prompt">$</span> Compiling program.ef...
              {'\n'}
              <span className="cursor-blink">▌</span>
            </pre>
          </div>
        ) : output ? (
          <pre className={`output-text ${output.includes('ERROR') || output.includes('❌') ? 'error-text' : 'success-text'}`}>
            <span className="prompt">$</span> compiler program.ef{'\n'}
            {'\n'}
            {output}
          </pre>
        ) : (
          <div className="output-empty">
            <div className="empty-icon">🖥️</div>
            <p className="empty-title">No output yet</p>
            <p className="empty-hint">
              Click <strong>Run</strong> or press <kbd>Ctrl</kbd> + <kbd>Enter</kbd> to compile your code
            </p>
          </div>
        )}
      </div>
    </div>
  );
}

export default Output;
