<?php
$servername = "localhost";
// REPLACE with your Database name
$dbname = "smartmo2_db_listrik";
// REPLACE with Database user
$username = "smartmo2_power_meter";
// REPLACE with Database user password
$password = "Smartpowermeter123!";
// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "nsdsuibs7xx278";
$api_key= $Tegangan = $Arus = $Daya_semu = $Faktor_daya = $Daya_aktif = $Daya_reaktif = $Frekuensi = $Energi = "";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $Tegangan = test_input($_POST["Tegangan"]);
        $Arus = test_input($_POST["Arus"]);
        $Daya_semu = test_input($_POST["Daya_semu"]);
        $Faktor_daya = test_input($_POST["Faktor_daya"]);
        $Daya_aktif = test_input($_POST["Daya_aktif"]);
        $Daya_reaktif = test_input($_POST["Daya_reaktif"]);
        $Frekuensi = test_input($_POST["Frekuensi"]);
        $Energi = test_input($_POST["Energi"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO Data_listrik (Tegangan, Arus, Daya_semu, Faktor_daya, Daya_aktif, Daya_reaktif, Frekuensi, Energi)
        VALUES ('" . $Tegangan . "', '" . $Arus . "', '" . $Daya_semu . "', '" . $Faktor_daya . "', '" . $Daya_aktif . "', '" . $Daya_reaktif . "', '" . $Frekuensi . "', '" . $Energi . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }}
else {
    echo "No data posted with HTTP POST.";
}function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}