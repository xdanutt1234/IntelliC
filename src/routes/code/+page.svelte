<script lang="ts">
    import { onMount } from "svelte";
    import { navigate } from "svelte-routing";
    import { user } from "../../javascript/authstore.js";
    import { get } from "svelte/store";
    import CodeMirror from "svelte-codemirror-editor";
    import { javascript } from "@codemirror/lang-javascript";
    import { python } from "@codemirror/lang-python";
    import { cpp } from "@codemirror/lang-cpp";
    import { db } from "../../javascript/firebase";
    import "../../javascript/firebase";
    import { logout } from "../../javascript/auth";
    import { collection, getDocs, query, where, setDoc, doc, updateDoc } from "firebase/firestore";
    import Navbar from "../../resources/Navbar.svelte";
    import { goto } from "$app/navigation";

    let problems = [];
    let problemlist = [];
    let title;
    let description;
    let problemId;
    let courseId;
    let value = "";
    let displayValue = "";
    let language = javascript();
    let isDropdownOpen = false;
    let currentUser = get(user);
    let submissionSuccess = false;

    async function getCourse() {
        const q = query(collection(db, "currentcourse"));
        const querySnapshot = await getDocs(q);
        problems = querySnapshot.docs.map((doc) => ({
            id: doc.id,
            ...doc.data(),
        }));
        courseId = problems[0].courseId;
    }

    async function getProblem() {
        const q = query(collection(db, "currentproblem"));
        const querySnapshot = await getDocs(q);
        problems = querySnapshot.docs.map((doc) => ({
            id: doc.id,
            ...doc.data(),
        }));
        problemId = problems[0].problemId;
    }

    async function setProblem(pid) {
        await setDoc(doc(db, "currentproblem", "wXdNfg2oO5quyBScNfhi"), {
            problemId: pid,
        });
        problemId = pid;
    }

    $: currentUser = $user;

    const handleLogout = () => {
        logout();
        navigate("/login");
    };

    async function sendMessage() {
        try {
            const socket = new WebSocket("ws://localhost:8080");

            socket.onopen = function () {
                let str =
                    currentUser.uid.slice(0, 6) + "@ahja@C@" + value + "@12@";
                socket.send(str);
            };

            socket.onerror = function (error) {
                console.error("WebSocket error:", error);
            };

            socket.onclose = function () {
                console.log("WebSocket connection closed.");
            };

            socket.onmessage = function (event: MessageEvent) {
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
    ];

    async function handleSolve(pid) {
        await setProblem(pid);
        await queryProblem();
        window.location.reload();
    }

    function toggleDropdown() {
        isDropdownOpen = !isDropdownOpen;
    }

    function changeLanguage(languageObj: any) {
        language = languageObj.value;
        isDropdownOpen = false;
    }

    async function debugProblems() {
        await getProblem();
        let allprob = [];
        const querySnapshot = await getDocs(collection(db, "exercises"));
        allprob = querySnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
        allprob.forEach(element => {
            if (element.id == problemId) {
                title = element.name;
                description = element.edescription;
            }
        });
    }

    async function queryProblems() {
        await getCourse();
        if (courseId) {
            const q = query(collection(db, "exercises"), where("courseId", "==", courseId));
            const querySnapshot = await getDocs(q);
            problemlist = querySnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
        }
    }

    async function queryProblem() {
        if (problemId) {
            try {
                const q = query(
                    collection(db, "exercises"),
                    where("id", "==", problemId),
                );
                const querySnapshot = await getDocs(q);
                problems = querySnapshot.docs.map((doc) => ({
                    id: doc.id,
                    ...doc.data(),
                }));
                if (problems.length > 0) {
                    title = problems[0].name;
                    description = problems[0].edescription;
                } else {
                    console.log("No problems found with the given ID.");
                }
            } catch (error) {
                console.error("Error querying Firestore:", error);
            }
        }
    }

    async function handleSubmit() {
        // Simulate correct output
        updateDisplay("Correct output!");
        // Highlight problem title
        submissionSuccess = true;
        // Add solved field to problem in Firestore
        if (problemId) {
            try {
                const problemRef = doc(db, "exercises", problemId);
                await updateDoc(problemRef, { solved: true });
                queryProblems(); // Refresh the problem list to reflect the solved state
            } catch (error) {
                console.error("Error updating Firestore:", error);
            }
        }
        setTimeout(() => {
            submissionSuccess = false; // Reset after a delay
        }, 3000);
    }

    onMount(() => {
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
        debugProblems();
        queryProblems();
        return () => {
            document.removeEventListener("click", handleClickOutside);
        };
    });
</script>

<Navbar />

<div class="problem-menu">
    <ul style="margin-top:60px">
        {#if problemlist.length > 0}
            {#each problemlist as problem}
                <button class="enroll_button" class:solved={problem.solved} on:click={() => handleSolve(problem.id)}>
                    {problem.name}
                </button>
            {/each}
        {/if}
    </ul>
</div>

<div class="coding-window">
    <div class="problem-container">
        <div class="problem-header" class:submission-success={submissionSuccess}>
            <h1>{title}</h1>
            <p>{description}</p>
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
                    height: "30rem",
                    backgroundColor: "#1e1e3f",
                },
            }}
        />
        <div class="terminal">
            <textarea
                readonly
                style="width: 100%; height: 300px; overflow: auto; resize: none;"
                >{displayValue}</textarea
            >
        </div>

        <div class="action-buttons">
            <button on:click={sendMessage}>Run</button>
            <button on:click={handleSubmit}>Submit</button>
        </div>
    </div>
</div>

<style>
    .problem-menu {
        position: fixed;
        left: 0;
        top: 0px;
        bottom: 0;
        height: 100%;
        width: 15%;
        overflow-y: auto;
        background-color: #333;
        color: #fff;
        padding: 10px;
        box-shadow: 2px 0 5px rgba(0, 0, 0, 0.2);
    }

    .problem-menu ul {
        list-style-type: none;
        padding: 0;
    }

    .problem-menu button {
        display: block;
        width: 100%;
        padding: 10px 15px;
        margin-bottom: 10px;
        background-color: #444;
        color: #fff;
        border: none;
        border-radius: 5px;
        text-align: left;
        cursor: pointer;
        transition: background-color 0.3s;
    }

    .problem-menu button.solved {
        background-color: #28a745;
    }

    .problem-menu button:hover {
        background-color: #555;
    }

    .problem-container {
        display: flex;
        flex-direction: column;
        margin-left: 20%;
        padding: 20px;
        width: 70%;
    }

    .problem-header {
        margin-bottom: 20px;
    }

    .problem-header.submission-success h1 {
        color: green;
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
        width: 100%;
        border: 1px solid #ccc;
        padding: 10px;
        margin-top: 20px;
        min-height: 100px;
    }

    .action-buttons {
        display: flex;
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
        transition: background-color 0.3s;
    }

    .action-buttons button:hover {
        background-color: #0056b3;
    }
</style>
