<?php
	// Die Klasse für den Datenbank-Zugriff wird benötigt
	require('classes/databasehandler.php');
?>

<html>
	<head>
		<meta content="text/html; charset=iso-8859-1" http-equiv="Content-Type">
		<link rel="stylesheet" type="text/css" href="site.css">
		<title>Raspberry Pi - Wetterstation - Lichtstärke</title>
	</head>
	<body>
		<?php include("menu.php"); ?>
		
		<h2>Lichtstärke</h2>
		
		<p>Aktuelle Lichtstärke: <?php echo DatabaseHandler::getLastAmbientLight(); ?></p>
		
		<p><img src="lichtstaerke_graph.php" alt="[Die letzten Temperatur-Messungen]"></p>
	</body>
</html>
