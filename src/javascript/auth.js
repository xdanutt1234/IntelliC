import { getAuth, signOut } from "firebase/auth";

// Get the auth instance
const auth = getAuth();

// Logout function
export const logout = async () => {
  try {
    await signOut(auth);
    console.log("User signed out successfully.");
    // Redirect or perform any other action after logout.
  } catch (error) {
    console.error("An error occurred while signing out:", error);
  }
};