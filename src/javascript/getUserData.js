// getUserData.js
import { doc, getDoc } from "firebase/firestore";
import { db } from "./firebaseConfig";

const getUserData = async (userId) => {
  try {
    const userDoc = await getDoc(doc(db, "users", userId));
    if (userDoc.exists()) {
      console.log('User data:', userDoc.data());
      return userDoc.data();
    } else {
      console.log('No such document!');
    }
  } catch (error) {
    console.error('Error getting user data:', error.message);
  }
};

export { getUserData };
