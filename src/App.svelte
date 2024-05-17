<script lang="ts">
    import { onMount } from "svelte";
    import CodeMirror from "svelte-codemirror-editor";
    import { javascript } from "@codemirror/lang-javascript";
    import { python } from "@codemirror/lang-python";
    import { cpp } from "@codemirror/lang-cpp";
    import './javascript/firebase';
    import { logout } from "./javascript/auth";
    import { navigate } from 'svelte-routing';
    import { Router, Route } from 'svelte-routing';
    import { auth } from './javascript/firebase';
    
    import Profile from './ProfilePage.svelte';
    import Login from './Login.svelte';

    let value = "#include <stdio.h>\nint main(){\n  return 0;\n}";
    let displayValue = "";
    let language = javascript();
    let isDropdownOpen = false;

    const handleProfile = () => {
        navigate('/profile');
    };

    const handleLogout = () => {
        logout();
        navigate('/login');
    };

    async function sendMessage() {
        console.log(value);
        try {
            const socket = new WebSocket("ws://localhost:8080");

            socket.onopen = function () {
                console.log("WebSocket connection established.");
                let str = "SA18lk@ahja@C@" + value + "@12@";
                console.log(str);
                socket.send(str);
            };

            socket.onerror = function (error) {
                console.error("WebSocket error:", error);
            };

            socket.onclose = function () {
                console.log("WebSocket connection closed.");
            };

            socket.onmessage = function (event: MessageEvent) {
                console.log("Received message:", event.data);
                const messageData: string = event.data.toString();
                if (messageData != "") {
                    updateDisplay(messageData);
                }
            };
        } catch (error) {
            console.error("WebSocket initialization error:", error);
        }
    }

    function updateDisplay(idk: string) {
        displayValue = idk;
    }

    const languages = [
        { label: "JavaScript", value: javascript() },
        { label: "Python", value: python() },
        { label: "C", value: cpp()},
        // Add more language options here
    ];

    function toggleDropdown() {
        isDropdownOpen = !isDropdownOpen;
    }

    function changeLanguage(languageObj: any) {
        language = languageObj.value;
        isDropdownOpen = false;
    }

    onMount(() => {
        // Close the dropdown when clicking outside of it
        const handleClickOutside = (event: any) => {
            if (
                isDropdownOpen &&
                !event.target.closest(".language-dropdown") &&
                !event.target.closest(".dropdown-content")
            ) {
                isDropdownOpen = false;
            }
        };

        document.addEventListener("click", handleClickOutside);

        return () => {
            document.removeEventListener("click", handleClickOutside);
        };
    });
</script>

<Router>
  <Route path="/login" component={Login} />
  <Route path="/profile" component={Profile} />
</Router>

<nav id="navbar" class="navbar">
    <button class="nav-link" on:click={() => navigate('/')}>Home</button>
    <button class="nav-link" on:click={() => navigate('/login')}>Courses</button>
    <button class="nav-link" on:click={() => navigate('/stats')}>Performance Stats</button>
    <button class="nav-link" on:click={() => navigate('/profile')}>Profile</button>
    <button class="nav-link" on:click={() => navigate('/login')}>Log Out</button>
</nav>

<div class="container">
    <div class="problem-header">
        <h1>Problem Title</h1>
        <p>Problem Description</p>
    </div>
    
    <div class="language-dropdown">
        <button class="dropdown-toggle" on:click={toggleDropdown}>
            Select Language
        </button>
        {#if isDropdownOpen}
            <div class="dropdown-content">
                {#each languages as languageObj}
                    <button on:click={() => changeLanguage(languageObj)}>
                        {languageObj.label}
                    </button>
                {/each}
            </div>
        {/if}
    </div>
    
    <div class="editor-container">
        <CodeMirror
            bind:value
            lang={language}
            styles={{
                "&": {
                    width: "100%",
                    height: "50rem",
                    backgroundColor: "#2b2b2b",
                    color: "#d4d4d4",
                    textAlign: "left" // Ensure text starts from the left
                },
                ".cm-content": {
                    textAlign: "left" // Align text inside the editor to the left
                }
            }}
        />
    </div>

    <div class="terminal">
        <textarea readonly>{displayValue}</textarea>
    </div>
    
    <div class="action-buttons">
        <button on:click={sendMessage}>Run</button>
    </div>
</div>

<style>
    /* Color palette */
    :root {
        --primary-bg-color: #2b2b2b;
        --primary-text-color: #d4d4d4;
        --secondary-bg-color: #333;
        --secondary-text-color: #ffffff;
        --accent-color: #007bff;
        --button-bg-color: #007bff;
        --button-text-color: #ffffff;
    }

    /* General layout styling */
    .container {
        margin: 0 auto;
        padding: 20px;
        max-width: 1200px;
        display: flex;
        flex-direction: column;
        align-items: center;
        color: var(--primary-text-color);
        background-color: var(--primary-bg-color);
    }

    .problem-header {
        text-align: center;
        margin-bottom: 20px;
    }

    .language-dropdown {
        position: relative;
        margin-bottom: 20px;
    }

    .dropdown-toggle {
        background-color: var(--primary-bg-color);
        border: 1px solid var(--secondary-bg-color);
        padding: 10px;
        cursor: pointer;
        border-radius: 5px;
        color: var(--primary-text-color);
    }

    .dropdown-content {
        display: block;
        position: absolute;
        background-color: var(--primary-bg-color);
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        border-radius: 5px;
        padding: 10px;
        top: 100%;
        left: 0;
        width: 150px;
        z-index: 1000;
    }

    .dropdown-content button {
        display: block;
        background-color: transparent;
        border: none;
        padding: 5px 0;
        width: 100%;
        text-align: left;
        cursor: pointer;
        color: var(--primary-text-color);
    }

    .editor-container {
        width: 100%;
        margin-bottom: 20px;
    }

    .terminal {
        width: 100%;
        border: 1px solid var(--secondary-bg-color);
        padding: 10px;
        margin-bottom: 20px;
        min-height: 200px;
    }

    .terminal textarea {
        width: 100%;
        height: 200px;
        resize: none;
        background-color: var(--primary-bg-color);
        border: none;
        padding: 10px;
        box-sizing: border-box;
        color: var(--primary-text-color);
    }

    .action-buttons {
        display: flex;
        justify-content: center;
        width: 100%;
    }

    .action-buttons button {
        padding: 10px 20px;
        border: none;
        background-color: var(--button-bg-color);
        color: var(--button-text-color);
        cursor: pointer;
        border-radius: 5px;
        margin: 0 10px;
    }

    .navbar {
        background-color: var(--secondary-bg-color);
        color: var(--secondary-text-color);
        padding: 10px;
        text-align: center;
        position: fixed;
        top: 0;
        width: 100%;
        z-index: 1000;
    }
</style>


