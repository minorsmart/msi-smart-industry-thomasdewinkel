// Initialize firestore
// Your web app's Firebase configuration
var firebaseConfig = {
apiKey: "AIzaSyCUk6-YcDJrcA3Vs6Z2mmGfDTNbD7xCyHE",
authDomain: "smartconnection-9d342.firebaseapp.com",
databaseURL: "https://smartconnection-9d342.firebaseio.com",
projectId: "smartconnection-9d342",
storageBucket: "smartconnection-9d342.appspot.com",
messagingSenderId: "467477051637",
appId: "1:467477051637:web:79703492fcd8c86889adbc"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);

var db = firebase.firestore();

function runFunction() {
    const n = document.getElementById("n-value").value
    const m = document.getElementById("m-value").value
    writeFunction(n,m)
}

function writeFunction(naam, bericht) {
    // Add a new document in collection "berichten"
    db.collection("berichten").add({
        naam: naam,
        bericht: bericht
    })
    .then(function() {
        console.log("Bericht succesvol verstuurd!");
    })
    .catch(function(error) {
        console.error("Fout: ", error);
    });
}
