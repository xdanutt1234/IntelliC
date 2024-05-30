<!-- src/routes/problems/[courseId].svelte -->
<script lang="ts">
    import { onMount } from "svelte";
    import { db } from "../../javascript/firebase"; // Import the Firestore instance from your firebase.js
    import {
        collection,
        getDocs,
        query,
        where,
        setDoc,
        doc,
    } from "firebase/firestore";
    import { user } from "../../javascript/authstore";
    import { page } from "$app/stores"; // Import the page store from SvelteKit
    import { currentproblem } from "../../javascript/current_problem";
    import { currentcourse } from "../../javascript/current_course";
    import { goto } from "$app/navigation";
    import { get } from "svelte/store";
    import Navbar from "../../resources/Navbar.svelte";
    let courseId;
    let problems = [];
    let currentUser = get(user);
    let isAdmin = [];

    // Extract the courseId from the URL
    currentcourse.subscribe((value) => {
        courseId = value;
    });
    function setProblem(problemId) {
        setDoc(doc(db, "currentproblem", "wXdNfg2oO5quyBScNfhi"), {
            problemId: problemId,
        })
            .then(() => console.log("Document successfully written!"))
            .catch((error) => console.error("Error writing document: ", error));
    }

    console.log(courseId);
    async function getCourse() {
        const q = query(collection(db, "currentcourse"));
        const querySnapshot = await getDocs(q);
        problems = querySnapshot.docs.map((doc) => ({
            id: doc.id,
            ...doc.data(),
        }));
        courseId = problems[0].courseId;
    }
    // Fetch problems from Firestore based on the courseId
    onMount(async () => {
        await getCourse();
        if (courseId) {
            const q = query(
                collection(db, "exercises"),
                where("courseId", "==", courseId),
            );
            const querySnapshot = await getDocs(q);
            problems = querySnapshot.docs.map((doc) => ({
                id: doc.id,
                ...doc.data(),
            }));
            console.log(problems);
        }

        if (currentUser?.uid) {
            const q = query(
                collection(db, "admins"),
                where("userId", "==", currentUser?.uid),
            );
            const querySnapshot = await getDocs(q);
            isAdmin = querySnapshot.docs.map((doc) => ({
                id: doc.id,
                ...doc.data(),
            }));
            console.log(isAdmin);
        }
    });

    const handleSolve = (pid) => {
        // Navigate to the problems list page with the course ID
        setProblem(pid);
        goto(`/code`);
    };
</script>

<head>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        .problems_container {
            margin-top: 100px;
            padding: 20px;
        }
        .problem_card {
            background-color: #f9f9f9;
            padding: 20px;
            margin-bottom: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        .problem_card h2 {
            margin-top: 0;
        }
        .enroll_button {
            background-color: #007bff;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s;
        }
        .enroll_button:hover {
            background-color: #0056b3;
        }
        .button-container {
            text-align: left;
            margin-left: 20px;
            margin-top: 100px;
        }
        
        .add-problem-button {
            background-color: #4caf50; /* Green */
            border: none;
            color: white;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 12px;
            transition:
                background-color 0.3s ease,
                transform 0.3s ease;
        }

        .add-problem-button:hover {
            background-color: #45a049;
            transform: scale(1.05);
        }

        .add-problem-button:active {
            background-color: #3e8e41;
            transform: scale(1);
        }
    </style>
</head>
<Navbar/>
<div class="button-container">
    <button class="add-problem-button">Add Problem</button>
</div>
<div class="problems_container">
    {#if problems.length > 0}
        {#each problems as problem}
            <div class="problem_card">
                <h2>{problem.name}</h2>
                <p>{problem.description}</p>
                <button
                    class="enroll_button"
                    on:click={() => handleSolve(problem.id)}>Solve</button
                >
            </div>
        {/each}
    {:else}
        <p>Loading problems...</p>
    {/if}
</div>
