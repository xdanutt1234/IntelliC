import { writable } from "svelte/store";
import { auth } from "./firebase";
import { onAuthStateChanged } from "firebase/auth";
import type { User } from "firebase/auth";

export const user = writable<User | null>(null);

onAuthStateChanged(auth, (currentUser) => {
    if (currentUser) {
        user.set(currentUser);
    } else {
        // Handle the case when currentUser is null
        // For example, you might want to set user to null
        user.set(null);
    }
});
