/**
 * About.jsx — Language Reference Modal
 *
 * Displays a clean modal overlay showing the Error-Friendly Language
 * rules, data types, keywords, operators, and syntax examples.
 * Closes on backdrop click, close button, or Escape key.
 */

import React, { useEffect } from 'react';
import './About.css';

function About({ isOpen, onClose }) {
  // Close on Escape key press
  useEffect(() => {
    const handleKey = (e) => {
      if (e.key === 'Escape') onClose();
    };
    if (isOpen) {
      window.addEventListener('keydown', handleKey);
      document.body.style.overflow = 'hidden';
    }
    return () => {
      window.removeEventListener('keydown', handleKey);
      document.body.style.overflow = '';
    };
  }, [isOpen, onClose]);

  if (!isOpen) return null;

  return (
    <div className="about-overlay" onClick={onClose}>
      <div className="about-modal" onClick={(e) => e.stopPropagation()}>
        {/* Modal Header */}
        <div className="about-header">
          <h2 className="about-title">Error-Friendly Language</h2>
          <p className="about-subtitle">Language Reference Guide</p>
          <button className="about-close" onClick={onClose} aria-label="Close">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round" strokeLinejoin="round">
              <line x1="18" y1="6" x2="6" y2="18" />
              <line x1="6" y1="6" x2="18" y2="18" />
            </svg>
          </button>
        </div>

        {/* Modal Body */}
        <div className="about-body">

          {/* Data Types */}
          <section className="about-section">
            <h3 className="section-title">📦 Data Types</h3>
            <div className="type-grid">
              <div className="type-card">
                <span className="type-name">int</span>
                <span className="type-desc">Integer numbers</span>
                <code className="type-example">int x = 42;</code>
              </div>
              <div className="type-card">
                <span className="type-name">float</span>
                <span className="type-desc">Decimal numbers</span>
                <code className="type-example">float pi = 3.14;</code>
              </div>
              <div className="type-card">
                <span className="type-name">bool</span>
                <span className="type-desc">True or false</span>
                <code className="type-example">bool flag = true;</code>
              </div>
              <div className="type-card">
                <span className="type-name">string</span>
                <span className="type-desc">Text values</span>
                <code className="type-example">string s = "Hi";</code>
              </div>
            </div>
          </section>

          {/* Keywords */}
          <section className="about-section">
            <h3 className="section-title">🔑 Keywords</h3>
            <div className="keyword-list">
              {['int', 'float', 'bool', 'string', 'if', 'else', 'while', 'for', 'true', 'false', 'return', 'void'].map((kw) => (
                <span key={kw} className="keyword-tag">{kw}</span>
              ))}
            </div>
          </section>

          {/* Operators */}
          <section className="about-section">
            <h3 className="section-title">⚙️ Operators</h3>
            <div className="operator-grid">
              <div className="op-group">
                <h4 className="op-group-title">Arithmetic</h4>
                <div className="op-list">
                  {['+', '-', '*', '/', '%'].map((op) => (
                    <span key={op} className="op-tag">{op}</span>
                  ))}
                </div>
              </div>
              <div className="op-group">
                <h4 className="op-group-title">Comparison</h4>
                <div className="op-list">
                  {['==', '!=', '<', '<=', '>', '>='].map((op) => (
                    <span key={op} className="op-tag">{op}</span>
                  ))}
                </div>
              </div>
              <div className="op-group">
                <h4 className="op-group-title">Logical</h4>
                <div className="op-list">
                  {['&&', '||', '!'].map((op) => (
                    <span key={op} className="op-tag">{op}</span>
                  ))}
                </div>
              </div>
              <div className="op-group">
                <h4 className="op-group-title">Assignment</h4>
                <div className="op-list">
                  <span className="op-tag">=</span>
                </div>
              </div>
            </div>
          </section>

          {/* Control Flow */}
          <section className="about-section">
            <h3 className="section-title">🔀 Control Flow</h3>
            <div className="syntax-examples">
              <div className="syntax-card">
                <h4>If / Else Statement</h4>
                <pre className="syntax-code">{`if (x > 5) {
    // do something
} else {
    // do something else
}`}</pre>
              </div>
              <div className="syntax-card">
                <h4>While Loop <span className="planned-tag">planned</span></h4>
                <pre className="syntax-code">{`while (x < 10) {
    x = x + 1;
}`}</pre>
              </div>
            </div>
          </section>

          {/* Variable Declaration */}
          <section className="about-section">
            <h3 className="section-title">📝 Variable Declaration</h3>
            <div className="syntax-card full-width">
              <pre className="syntax-code">{`// Declare without initialization
int count;

// Declare with initialization
float temperature = 98.6;
bool isReady = true;
string name = "Hello World";`}</pre>
            </div>
          </section>

          {/* Rules */}
          <section className="about-section">
            <h3 className="section-title">📋 Language Rules</h3>
            <ul className="rules-list">
              <li>Every statement must end with a semicolon <code>;</code></li>
              <li>Variables must be declared before use</li>
              <li>Variable names cannot be a reserved keyword</li>
              <li>Strings must be enclosed in double quotes <code>"..."</code></li>
              <li>Code blocks use curly braces <code>{'{ }'}</code></li>
              <li>Single-line comments start with <code>//</code></li>
              <li>Variables cannot be redeclared in the same scope</li>
              <li>File extension: <code>.ef</code></li>
            </ul>
          </section>

          {/* Error Messages */}
          <section className="about-section">
            <h3 className="section-title">💡 Error Messages</h3>
            <p className="section-desc">
              The compiler provides clear, beginner-friendly errors with line/column info, explanations, and fix suggestions.
            </p>
            <div className="syntax-card full-width error-example">
              <pre className="syntax-code error-code">{`[Line 3, Column 5] UNDECLARED VARIABLE ERROR
  Variable 'x' is not declared.
  Context: x = 42;
  Suggestion: Declare the variable before using it: int x;`}</pre>
            </div>
          </section>

        </div>
      </div>
    </div>
  );
}

export default About;
