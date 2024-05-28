<!-- src/routes/problems/[courseId].svelte -->
<script lang="ts">
    import { onMount } from "svelte";
    import { db } from "../../javascript/firebase"; // Import the Firestore instance from your firebase.js
    import { collection, getDocs, query, where } from "firebase/firestore";
    import { page } from "$app/stores"; // Import the page store from SvelteKit
    import { currentcourse } from "../../javascript/current_course";
    let courseId;
    let problems = [];

    // Extract the courseId from the URL
    currentcourse.subscribe(value => {
        courseId = value;
    });
    
    console.log(courseId);

    // Fetch problems from Firestore based on the courseId
    onMount(async () => {
        if (courseId) {
            const q = query(collection(db, "exercises"), where("courseId", "==", courseId));
            const querySnapshot = await getDocs(q);
            problems = querySnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
            console.log(problems);
        }
    });
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
    </style>
</head>

<div class="problems_container">
    {#if problems.length > 0}
        {#each problems as problem}
            <div class="problem_card">
                <h2>{problem.name}</h2>
                <p>{problem.description}</p>
            </div>
        {/each}
    {:else}
        <p>Loading problems...</p>
    {/if}
</div>
