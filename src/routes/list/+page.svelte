<script lang="ts">
    import { onMount } from "svelte";
    import Navbar from "../../resources/Navbar.svelte";
    import { db } from "../../javascript/firebase"; // Import the Firestore instance from your firebase.js
    import { collection, getDocs, setDoc, doc, query, where } from "firebase/firestore";
    import { goto } from "$app/navigation"; // Import the goto function from SvelteKit
    import { currentcourse } from "../../javascript/current_course";

    let courses = [];
    let problemsData = {};

    function setCourse(courseId) {
        setDoc(doc(db, "currentcourse", "P7pVp38246bjAc5HftOQ"), { courseId: courseId })
            .then(() => console.log("Document successfully written!"))
            .catch((error) => console.error("Error writing document: ", error));
    }

    // Fetch courses and problem data from Firestore on component mount
    onMount(async () => {
        const coursesSnapshot = await getDocs(collection(db, "courses"));
        courses = coursesSnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));

        const exercisesSnapshot = await getDocs(collection(db, "exercises"));
        problemsData = exercisesSnapshot.docs.reduce((acc, doc) => {
            const data = doc.data();
            const courseId = data.courseId;
            if (!acc[courseId]) {
                acc[courseId] = { total: 0, solved: 0 };
            }
            acc[courseId].total += 1;
            if (data.solved) {
                acc[courseId].solved += 1;
            }
            return acc;
        }, {});
    });

    const handleEnroll = (courseId) => {
        console.log(`Enroll in course with ID: ${courseId}`);
        setCourse(courseId);
        goto(`/problems`);
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
                <p>Total Problems: {problemsData[course.id]?.total || 0}</p>
                <p>Solved Problems: {problemsData[course.id]?.solved || 0}</p>
                <button class="enroll_button" on:click={() => handleEnroll(course.id)}>Enroll</button>
            </div>
        {/each}
    {:else}
        <p>Loading courses...</p>
    {/if}
</div>
