<script lang="ts">
    import { onMount } from "svelte";
    import { navigate } from "svelte-routing";

    import CodeMirror from "svelte-codemirror-editor";
    import { javascript } from "@codemirror/lang-javascript";
    import { python } from "@codemirror/lang-python";
    import { cpp } from "@codemirror/lang-cpp";
    import "../../javascript/firebase";
    import { logout } from "../../javascript/auth";

    import Navbar from "../../resources/Navbar.svelte";

    let value = "";
    let displayValue = "";
    let language = javascript();
    let isDropdownOpen = false;

    const handleLogout = () => {
        logout();
        navigate("/login");
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
        { label: "C", value: cpp() },
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
        const handleClickOutside = (event: MouseEvent) => {
            const target = event.target as Element;
            if (
                isDropdownOpen &&
                !target.closest(".language-dropdown") &&
                !target.closest(".dropdown-content")
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

<Navbar />

<div
    class="problem-menu"
    style="position: fixed; left: 0; top: 0px; bottom: 0; height: 100%; width: 10%; overflow-y: auto; background-color: #333; color: #fff; padding: 10px;"
>
    <ul style="margin-top: 40px;">
        <li>Problem 1</li>
        <li>Problem 2</li>
        <!-- Add more problems here -->
        <!-- You might need to adjust the styling based on your specific layout -->
    </ul>
</div>

<!-- Coding window (centered) -->
<div class="coding-window" style="position: absolute; left: 300px; top: 50px;">
    <div
        class="problem-container"
        style="display: flex; flex-direction: column;"
    >
        <!-- Problem title and description (to be populated from the database) -->
        <div class="problem-header">
            <h1 style="text-align: center;">Problem Title</h1>
            <p>Problem Description</p>
        </div>
        <div class="language-dropdown">
            <button class="dropdown-toggle" on:click={toggleDropdown}>
                Select Language
            </button>
            {#if isDropdownOpen}
                <div class="dropdown-content">
                    {#each languages as languageObj}
                        <button on:click={() => changeLanguage(languageObj)}
                            >{languageObj.label}</button
                        >
                    {/each}
                </div>
            {/if}
        </div>

        <CodeMirror
            bind:value
            lang={language}
            styles={{
                "&": {
                    width: "1080px",
                    maxWidth: "1080px",
                    height: "50rem",
                    backgroundColor: "#1e1e3f",
                },
            }}
        />
        <div class="terminal">
            <!-- Add terminal content here -->
            <textarea
                readonly
                style="width: 100%; height: 300px; overflow: auto; resize: none;"
                >{displayValue}</textarea
            >
        </div>

        <!-- Buttons for running and submitting code -->
        <div class="action-buttons">
            <button on:click={sendMessage}>Run</button>
            <button>Submit</button>
        </div>
    </div>
</div>

<!-- Terminal-like area for code execution feedback -->

<style>
    /* Styling for the layout */
    .problem-container {
        display: flex;
        justify-content: space-between;
        align-items: flex-start;
        padding: 20px;
        margin-top: 50px; /* Adjusted to accommodate the navbar */
    }

    .problem-header {
        width: 70%; /* Adjust width as needed */
    }

    .problem-menu {
        width: 20%; /* Adjust width as needed */
    }

    .coding-window {
        width: 70%; /* Adjust width as needed */
        display: flex;
        flex-direction: column;
        align-items: center;
    }

    .language-dropdown {
        position: relative;
        margin-bottom: 20px;
    }

    .dropdown-toggle {
        background-color: #f9f9f9;
        border: none;
        padding: 10px;
        cursor: pointer;
        border-radius: 5px;
    }

    .dropdown-content {
        display: block;
        position: absolute;
        background-color: #fff;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        border-radius: 5px;
        padding: 10px;
        top: 100%;
        left: 0;
        width: 120px;
        right: 0; /* Aligns the dropdown to the right */
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
    }

    .terminal {
        width: 70%; /* Adjust width as needed */
        border: 1px solid #ccc;
        padding: 10px;
        margin-left: 180px;
        margin-top: 20px;
        min-height: 100px; /* Adjust height as needed */
    }

    .action-buttons {
        width: 70%; /* Adjust width as needed */
        display: flex;
        margin-left: 500px;
        margin-bottom: 70px;
        justify-content: center;
        margin-top: 20px;
    }

    .action-buttons button {
        margin: 0 10px;

        padding: 10px 20px;
        border: none;
        background-color: #007bff;
        color: #fff;
        cursor: pointer;
        border-radius: 5px;
    }
</style>
