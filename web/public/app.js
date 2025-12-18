const socket = io();
const terminal = document.getElementById('terminal');
const manualInput = document.getElementById('manual-input');
const statusIndicator = document.getElementById('connection-status');
const demoOverlay = document.getElementById('demo-overlay');
const typingIndicator = document.getElementById('typing-indicator');
const timerElement = document.getElementById('session-timer');

let timerInterval;
const SESSION_TIME = 5 * 60; // seconds

// Connection Handling
socket.on('connect', () => {
    console.log('Connected to server');
    updateStatus(true);
    startTimer();
    socket.emit('start-session');
});

socket.on('disconnect', () => {
    console.log('Disconnected');
    updateStatus(false);
    stopTimer();
});

socket.on('session-ended', (msg) => {
    alert(msg);
    window.location.reload();
});

function startTimer() {
    let timeLeft = SESSION_TIME;
    updateTimerDisplay(timeLeft);
    
    clearInterval(timerInterval);
    timerInterval = setInterval(() => {
        timeLeft--;
        updateTimerDisplay(timeLeft);
        
        if (timeLeft <= 0) {
            clearInterval(timerInterval);
            timerElement.classList.add('text-red-500', 'animate-pulse');
        } else if (timeLeft <= 60) {
            timerElement.classList.remove('text-blue-400');
            timerElement.classList.add('text-yellow-500');
        }
    }, 1000);
}

function stopTimer() {
    clearInterval(timerInterval);
    timerElement.textContent = "--:--";
}

function updateTimerDisplay(seconds) {
    const m = Math.floor(seconds / 60).toString().padStart(2, '0');
    const s = (seconds % 60).toString().padStart(2, '0');
    timerElement.textContent = `${m}:${s}`;
}

// Terminal Output Handling
socket.on('terminal-output', (data) => {
    // Hide typing indicator when data arrives
    typingIndicator.classList.add('hidden');
    
    let isNewScreen = false;

    // Check for Clear Screen marker
    if (data.includes('<<CLS>>')) {
        terminal.innerHTML = '';
        // Remove the marker and leading/trailing whitespace/newlines from the new content
        data = data.replace('<<CLS>>', '').trimStart();
        isNewScreen = true;
    }
    
    if (data) {
        const span = document.createElement('span');
        span.textContent = data;
        terminal.appendChild(span);
        
        if (isNewScreen) {
            // If it's a fresh screen (like a report), jump to TOP to let user read from start
            terminal.scrollTop = 0;
        } else {
            // Otherwise (interactive typing), jump to BOTTOM
            terminal.scrollTop = terminal.scrollHeight;
        }
        
        if (terminal.childElementCount > 1000) {
            terminal.removeChild(terminal.firstChild);
        }
    }
});

// Command Sending
function sendCommand(cmd, btnElement) {
    // Visual Feedback
    if (btnElement) {
        btnElement.classList.add('clicked');
        setTimeout(() => btnElement.classList.remove('clicked'), 200);
    }
    
    // Show typing indicator
    typingIndicator.classList.remove('hidden');
    
    socket.emit('send-input', cmd);
}

function sendManualInput() {
    const val = manualInput.value;
    
    if (val === '') {
        // Visual feedback for Enter key
        const span = document.createElement('span');
        span.textContent = '\n> [ENTER Key Sent]\n';
        span.classList.add('text-gray-500', 'italic');
        terminal.appendChild(span);
        terminal.scrollTop = terminal.scrollHeight;
        
        sendCommand('');
    } else {
        sendCommand(val);
    }
    
    manualInput.value = '';
}

// UI Helpers
function updateStatus(online) {
    if (online) {
        statusIndicator.className = "flex items-center space-x-2 text-xs text-green-400 bg-green-900/20 px-3 py-1.5 rounded-full transition-colors duration-500";
        statusIndicator.innerHTML = '<span class="w-2 h-2 rounded-full bg-current animate-pulse"></span><span>System Online</span>';
    } else {
        statusIndicator.className = "flex items-center space-x-2 text-xs text-red-500 bg-red-900/20 px-3 py-1.5 rounded-full transition-colors duration-500";
        statusIndicator.innerHTML = '<span class="w-2 h-2 rounded-full bg-current"></span><span>Offline</span>';
    }
}

function closeDemo() {
    demoOverlay.classList.add('opacity-0', 'pointer-events-none', 'transition-opacity', 'duration-500');
}

function showDemo() {
    demoOverlay.classList.remove('opacity-0', 'pointer-events-none');
}

// Event Listeners
document.getElementById('manual-form').addEventListener('submit', (e) => {
    e.preventDefault();
    sendManualInput();
});

document.getElementById('reset-btn').addEventListener('click', () => {
    terminal.innerHTML = '';
    socket.emit('start-session');
});

document.getElementById('clear-btn').addEventListener('click', () => {
    terminal.innerHTML = '';
});

// Theme Handling
const themeToggle = document.getElementById('theme-toggle');
const sunIcon = document.getElementById('sun-icon');
const moonIcon = document.getElementById('moon-icon');
const body = document.body;
const header = document.querySelector('header');
const panels = document.querySelectorAll('.bg-gray-800\\/50'); // Left panels
const buttons = document.querySelectorAll('button');

// Check system preference
const systemPrefersDark = window.matchMedia('(prefers-color-scheme: dark)').matches;
let isDarkMode = localStorage.getItem('theme') === 'dark' || (!localStorage.getItem('theme') && systemPrefersDark);

function applyTheme(dark) {
    if (dark) {
        // Dark Mode
        body.classList.remove('bg-gray-100', 'text-gray-900');
        body.classList.add('bg-gray-900', 'text-gray-100');
        
        header.classList.remove('bg-white', 'border-gray-200');
        header.classList.add('bg-gray-900/50', 'border-gray-800');
        
        panels.forEach(p => {
            p.classList.remove('bg-white', 'border-gray-200', 'shadow-md');
            p.classList.add('bg-gray-800/50', 'border-gray-700');
        });

        sunIcon.classList.add('hidden');
        moonIcon.classList.remove('hidden');
        localStorage.setItem('theme', 'dark');
    } else {
        // Light Mode
        body.classList.remove('bg-gray-900', 'text-gray-100');
        body.classList.add('bg-gray-100', 'text-gray-900');
        
        header.classList.remove('bg-gray-900/50', 'border-gray-800');
        header.classList.add('bg-white', 'border-gray-200');
        
        panels.forEach(p => {
            p.classList.remove('bg-gray-800/50', 'border-gray-700');
            p.classList.add('bg-white', 'border-gray-200', 'shadow-md');
        });

        moonIcon.classList.add('hidden');
        sunIcon.classList.remove('hidden');
        localStorage.setItem('theme', 'light');
    }
}

// Initial Apply
applyTheme(isDarkMode);

// Toggle Listener
themeToggle.addEventListener('click', () => {
    isDarkMode = !isDarkMode;
    applyTheme(isDarkMode);
});

// Font Size Toggling
// Order: Tiny -> Small -> Normal -> Tiny
const fontSizes = ['text-[10px]', 'text-xs', 'text-sm'];
let currentFontIndex = 0; // Starts at text-[10px]

document.getElementById('font-btn').addEventListener('click', () => {
    // Remove current class
    terminal.classList.remove(fontSizes[currentFontIndex]);
    
    // Cycle index
    currentFontIndex = (currentFontIndex + 1) % fontSizes.length;
    
    // Add new class
    terminal.classList.add(fontSizes[currentFontIndex]);
});
