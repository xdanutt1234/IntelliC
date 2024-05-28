<script lang="ts">
    import { onMount } from "svelte";
    import Navbar from "../../resources/Navbar.svelte";
    import { db } from "../../javascript/firebase"; // Import the Firestore instance from your firebase.js
    import { collection, getDocs } from "firebase/firestore";

    let courses = [];

    // Fetch courses from Firestore on component mount
    onMount(async () => {
        const querySnapshot = await getDocs(collection(db, "courses"));
        courses = querySnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
    });

    const handleEnroll = (courseId) => {
        console.log(`Enroll in course with ID: ${courseId}`);
    };
</script>

<head>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        .courses_container {
            margin-top: 100px;
            padding: 20px;
        }
        .course_card {
            background-color: #f9f9f9;
            padding: 20px;
            margin-bottom: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        .course_card h2 {
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
    </style>
</head>

<Navbar />

<div class="courses_container">
    {#if courses.length > 0}
        {#each courses as course}
            <div class="course_card">
                <h2>{course.name}</h2>
                <p>{course.description}</p>
                <button class="enroll_button" on:click={() => handleEnroll(course.id)}>Enroll</button>
            </div>
        {/each}
    {:else}
        <p>Loading courses...</p>
    {/if}
</div>
