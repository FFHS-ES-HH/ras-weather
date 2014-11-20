<?php
	// Die Klasse für den Datenbank-Zugriff wird benötigt
	require('classes/databasehandler.php');
	
	require_once ("/usr/share/jpgraph/jpgraph.php");
	require_once ("/usr/share/jpgraph/jpgraph_line.php");
?>

<html>
	<head>
		<meta content="text/html; charset=iso-8859-1" http-equiv="Content-Type">
		<link rel="stylesheet" type="text/css" href="site.css">
		<title>Raspberry Pi - Wetterstation - Temperatur</title>
	</head>
	<body>
		<?php include("menu.php"); ?>
		
		<h2>Temperatur</h2>
		
		<p>Aktuelle Temperatur: <?php echo DatabaseHandler::getLastTemperature(); ?></p>
		
		<p><img src="temperatur_graph.php" alt="[Die letzten Temperatur-Messungen]"></p>
	</body>
</html>
