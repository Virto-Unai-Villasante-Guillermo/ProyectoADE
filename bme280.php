<?php

// Importamos la configuracion
require("config.php");

// Leemos los valores que nos llegan por GET
$Temperatura = mysqli_real_escape_string($con, $_GET['Temperatura']);
$Presion = mysqli_real_escape_string($con, $_GET['Presion']);
$Humedad = mysqli_real_escape_string($con, $_GET['Humedad']);

// EInsertamos los valores en la tabla
$query = "INSERT INTO Temp_Press_Hum (Temperatura, Presion, Humedad) VALUES ('$Temperatura', '$Presion', '$Humedad')";

// Ejecutamos la instruccion
mysqli_query($con, $query);

mysqli_close($con);

echo "Pagina para subir los datos<br />";
echo "<br />Temperatura = $Temperatura ºC<br />";
echo "<br />Humedad = $Humedad %<br />";

// Conexión a la base de datos
$servername = "localhost"; // Cambia esto por la dirección de tu servidor de base de datos
$username = "unai"; // Cambia esto por el nombre de usuario de tu base de datos
$password = "1221"; // Cambia esto por la contraseña de tu base de datos
$database = "BME280"; // Cambia esto por el nombre de tu base de datos

// Crear conexión
$conn = new mysqli($servername, $username, $password, $database);

// Verificar conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Consulta SQL para recuperar los datos
$sql = "SELECT Temperatura, Presion, Humedad FROM Temp_Press_Hum";
$result = $conn->query($sql);

// Mostrar los datos en formato HTML
if ($result->num_rows > 0) {
    // Output data of each row
    while($row = $result->fetch_assoc()) {
        echo "Temperatura: " . $row["Temperatura"]. " - Presion: " . $row["Presion"]. " - Humedad: " . $row["Humedad"]. "<br>";
    }
} else {
    echo "0 resultados";
}

// Cerrar conexión
$conn->close();
?>
