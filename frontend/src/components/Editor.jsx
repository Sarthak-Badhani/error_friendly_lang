/**
 * Editor.jsx — Monaco Editor Wrapper Component
 *
 * Renders the Monaco code editor with dark theme, line numbers,
 * and basic syntax highlighting. Exposes the current code value
 * to the parent via the `onChange` callback.
 */

import React from 'react';
import MonacoEditor from '@monaco-editor/react';
import './Editor.css';

// Default placeholder code shown when the editor loads
const DEFAULT_CODE = `// Write your code here
// Example: Error-Friendly Language

int x = 10;
float pi = 3.14;
string greeting = "Hello, World!";

if (x > 5) {
    bool flag = true;
}
`;

function Editor({ code, onChange }) {
  /**
   * Called whenever the user edits code in the editor.
   * Passes the updated code string back to the parent component.
   */
  const handleEditorChange = (value) => {
    onChange(value || '');
  };

  /**
   * Configure extra editor options once mounted.
   * We keep a reference to the editor instance for potential future use
   * (e.g., programmatic text insertion, formatting, etc.)
   */
  const handleEditorMount = (editor, monaco) => {
    // Focus the editor on mount for immediate typing
    editor.focus();

    // Register a custom theme with the compiler's brand colors
    monaco.editor.defineTheme('compilerDark', {
      base: 'vs-dark',
      inherit: true,
      rules: [
        { token: 'comment', foreground: '6A9955', fontStyle: 'italic' },
        { token: 'keyword', foreground: 'C586C0' },
        { token: 'string', foreground: 'CE9178' },
        { token: 'number', foreground: 'B5CEA8' },
        { token: 'type', foreground: '4EC9B0' },
      ],
      colors: {
        'editor.background': '#0f1525',
        'editor.foreground': '#e2e8f0',
        'editorCursor.foreground': '#6366f1',
        'editor.lineHighlightBackground': '#1a2240',
        'editorLineNumber.foreground': '#475569',
        'editorLineNumber.activeForeground': '#94a3b8',
        'editor.selectionBackground': '#6366f140',
        'editor.inactiveSelectionBackground': '#6366f120',
        'editorIndentGuide.background': '#1e293b',
        'editorIndentGuide.activeBackground': '#334155',
      },
    });

    monaco.editor.setTheme('compilerDark');
  };

  return (
    <div className="editor-container">
      {/* Editor header with file tab */}
      <div className="editor-header">
        <div className="editor-tab active">
          <span className="tab-icon">📄</span>
          <span className="tab-name">program.ef</span>
        </div>

      </div>

      {/* Monaco Editor instance */}
      <div className="editor-body">
        <MonacoEditor
          height="100%"
          defaultLanguage="cpp" // Closest built-in syntax for our language
          defaultValue={DEFAULT_CODE}
          value={code}
          onChange={handleEditorChange}
          onMount={handleEditorMount}
          theme="vs-dark" // Fallback; custom theme applied on mount
          loading={
            <div className="editor-loading">
              <div className="editor-loading-spinner" />
              <span>Loading editor...</span>
            </div>
          }
          options={{
            fontSize: 14,
            fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
            fontLigatures: true,
            lineNumbers: 'on',
            minimap: { enabled: false },
            scrollBeyondLastLine: false,
            wordWrap: 'on',
            automaticLayout: true,
            padding: { top: 16, bottom: 16 },
            cursorBlinking: 'smooth',
            cursorSmoothCaretAnimation: 'on',
            smoothScrolling: true,
            renderWhitespace: 'selection',
            bracketPairColorization: { enabled: true },
            tabSize: 4,
            insertSpaces: true,
            suggest: { showKeywords: true },
            contextmenu: true,
          }}
        />
      </div>
    </div>
  );
}

// Export component and default code for use in App.jsx
export { DEFAULT_CODE };
export default Editor;
