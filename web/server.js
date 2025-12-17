const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const { spawn } = require('child_process');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const PORT = process.env.PORT || 3000;

// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

// Path to the compiled C++ executable
const MSH_EXECUTABLE = path.join(__dirname, '..', 'build', 'bin', 'msh');

let mshProcess = null;

io.on('connection', (socket) => {
    console.log('A user connected');

    socket.on('start-session', () => {
        if (mshProcess) {
            mshProcess.kill();
        }

        console.log(`Spawning ${MSH_EXECUTABLE}...`);
        
        // Spawn the C++ process
        // We assume the C++ app is built and located at ../build/bin/msh
        // Dockerfile will ensure this structure.
        try {
            mshProcess = spawn(MSH_EXECUTABLE, [], {
                stdio: ['pipe', 'pipe', 'pipe'] // Pipe stdin, stdout, stderr
            });

            mshProcess.stdout.on('data', (data) => {
                const output = data.toString();
                socket.emit('terminal-output', output);
            });

            mshProcess.stderr.on('data', (data) => {
                const output = data.toString();
                socket.emit('terminal-output', `[STDERR] ${output}`);
            });

            mshProcess.on('close', (code) => {
                socket.emit('terminal-output', `\n[Process exited with code ${code}]`);
                mshProcess = null;
            });

            mshProcess.on('error', (err) => {
                 socket.emit('terminal-output', `\n[Error spawning process: ${err.message}]\nIs the C++ app compiled?`);
            });

        } catch (e) {
            socket.emit('terminal-output', `\n[Exception] ${e.message}`);
        }
    });

    socket.on('send-input', (input) => {
        if (mshProcess && mshProcess.stdin) {
            // Write input to the C++ process stdin
            // Append newline as std::cin usually waits for it
            mshProcess.stdin.write(input + '\n');
        }
    });

    socket.on('disconnect', () => {
        console.log('User disconnected');
        if (mshProcess) {
            mshProcess.kill();
            mshProcess = null;
        }
    });
});

server.listen(PORT, '0.0.0.0', () => {
    console.log(`Server listening on port ${PORT}`);
});
