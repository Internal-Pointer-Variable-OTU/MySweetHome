const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const { spawn } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const PORT = process.env.PORT || 3000;

// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

// Path to the compiled C++ executable
const MSH_EXECUTABLE = path.join(__dirname, '..', 'build', 'bin', 'msh');
const SESSIONS_DIR = path.join(__dirname, 'sessions');
const SESSION_DURATION_MS = 5 * 60 * 1000; // 5 minutes

// Ensure sessions directory exists
if (!fs.existsSync(SESSIONS_DIR)) {
    fs.mkdirSync(SESSIONS_DIR, { recursive: true });
}

io.on('connection', (socket) => {
    console.log(`A user connected: ${socket.id}`);
    
    let userProcess = null;
    let sessionTimeout = null;
    const userSessionDir = path.join(SESSIONS_DIR, socket.id);

    // Start session timer
    sessionTimeout = setTimeout(() => {
        console.log(`Session timed out for ${socket.id}`);
        socket.emit('session-ended', 'Time limit reached (5 min). Please refresh to restart.');
        cleanupSession();
        socket.disconnect(true);
    }, SESSION_DURATION_MS);

    function cleanupSession() {
        if (userProcess) {
            userProcess.kill();
            userProcess = null;
        }
        // Small delay to ensure process releases file locks
        setTimeout(() => {
            try {
                if (fs.existsSync(userSessionDir)) {
                    fs.rmSync(userSessionDir, { recursive: true, force: true });
                }
            } catch (e) {
                console.error(`Failed to cleanup dir for ${socket.id}:`, e.message);
            }
        }, 100);
    }

    socket.on('start-session', () => {
        if (userProcess) {
            userProcess.kill();
        }

        // Create isolated directory for this session
        try {
            if (!fs.existsSync(userSessionDir)) {
                fs.mkdirSync(userSessionDir);
            }
        } catch (e) {
            socket.emit('terminal-output', `\n[Error] Could not create session workspace: ${e.message}`);
            return;
        }

        console.log(`Spawning ${MSH_EXECUTABLE} for ${socket.id} in ${userSessionDir}`);
        
        try {
            userProcess = spawn(MSH_EXECUTABLE, [], {
                stdio: ['pipe', 'pipe', 'pipe'],
                cwd: userSessionDir // Run in isolated directory
            });

            userProcess.stdout.on('data', (data) => {
                const output = data.toString();
                socket.emit('terminal-output', output);
            });

            userProcess.stderr.on('data', (data) => {
                const output = data.toString();
                socket.emit('terminal-output', `[STDERR] ${output}`);
            });

            userProcess.on('close', (code) => {
                socket.emit('terminal-output', `\n[Process exited with code ${code}]`);
                userProcess = null;
            });

            userProcess.on('error', (err) => {
                 socket.emit('terminal-output', `\n[Error spawning process: ${err.message}]\nIs the C++ app compiled?`);
            });

        } catch (e) {
            socket.emit('terminal-output', `\n[Exception] ${e.message}`);
        }
    });

    socket.on('send-input', (input) => {
        const sanitizedInput = input.toString().trim();

        // Allow empty input (Enter key)
        if (sanitizedInput === '') {
            if (userProcess && userProcess.stdin) {
                userProcess.stdin.write('\n');
            }
            return;
        }

        // SECURITY: Input Validation
        // Allow only:
        // 1. Numbers (any length, for quantity input)
        // 2. Single letters (a-z, A-Z) for menu choices
        // 3. '10' is covered by numbers rule.
        const allowedPattern = /^(\d+|[a-zA-Z])$/;

        if (!allowedPattern.test(sanitizedInput)) {
            socket.emit('terminal-output', `\n[SECURITY BLOCK] Invalid command: "${sanitizedInput}"\nOnly numbers and single letters are allowed.\n`);
            return;
        }

        if (userProcess && userProcess.stdin) {
            // Write input to the C++ process stdin
            userProcess.stdin.write(sanitizedInput + '\n');
        }
    });

    socket.on('disconnect', () => {
        console.log(`User disconnected: ${socket.id}`);
        if (sessionTimeout) clearTimeout(sessionTimeout);
        cleanupSession();
    });
});

server.listen(PORT, '0.0.0.0', () => {
    console.log(`Server listening on port ${PORT}`);
});
