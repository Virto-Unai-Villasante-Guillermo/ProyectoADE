<?php
// Conexión a la base de datos
$servername = "localhost";
$username = "unai";
$password = "1221"; 
$database = "BME280";

// Crear conexión
$conn = new mysqli($servername, $username, $password, $database);

// Verificar conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Consulta SQL para recuperar los datos
$sql = "SELECT Temperatura, Presion, Humedad, Fecha FROM Temp_Press_Hum";
$result = $conn->query($sql);

// Mostrar los datos en formato HTML
if ($result->num_rows > 0) {
    // Output data of each row
    while($row = $result->fetch_assoc()) {
	echo "Fecha y hora: " . $row["Fecha"] . " Temperatura: " . $row["Temperatura"]. "ºC - Presion: " . $row["Presion"]. "hPa - Humedad: " . $row["Humedad"]. "%<br>";
    }
} else {
    echo "0 resultados";
}

// Cerrar conexión
$conn->close();
?>

<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Visualización de Datos</title>
</head>
<body>

<div id="datos-container">
    <!-- Aquí se mostrarán los datos -->
</div>

<script>
// Función para cargar y mostrar los datos
function cargarDatos() {
    // Realizar una solicitud AJAX para obtener los datos
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            // Actualizar el contenido de datos-container con los nuevos datos
            document.getElementById("datos-container").innerHTML = this.responseText;
        }
    };
    xhr.open("GET","datos.php", true);
    xhr.send();
}

// Cargar datos inicialmente
cargarDatos();

// Actualizar los datos cada segundo
setInterval(cargarDatos, 1000); // 1000 milisegundos = 1 segundos
</script>

</body>
</html>

