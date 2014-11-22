<?php
	// Die Klasse für den Datenbank-Zugriff wird benötigt
	require('classes/databasehandler.php');
?>

<html>
	<head>
		<meta content="text/html; charset=utf-8" http-equiv="Content-Type">
		<link rel="stylesheet" type="text/css" href="site.css">
		<title>Raspberry Pi - Wetterstation - Lichtstärke</title>
	</head>
	<body>
		<table cellpadding="0" cellspacing="0" style="width:100%">
			<tr>
				<td style="width: 130px"><img src="images/raspberry.png" alt="Raspberry Pi" width="109px" height="97px"></td>
				<td style="font-size: 20px; font-weight: bold; text-align:left">Raspberry Pi - Wetterstation</td>
			</tr>
			<tr>
				<td colspan="2">
					<table cellpadding="0" cellspacing="0">
						<tr style="height: 43px">
							<td class="NavigationTab" style="cursor: pointer" onclick="document.location.href='temperatur.php';">
							Temperatur</td>
							<td style="width: 5px"></td>
							<td class="NavigationTab" style="cursor: pointer" onclick="document.location.href='feuchtigkeit.php';">
							Luftfeuchtigkeit</td>
							<td style="width: 5px"></td>
							<td class="NavigationTab" style="cursor: pointer" onclick="document.location.href='druck.php';">
							Luftdruck</td>
							<td style="width: 5px"></td>
							<td class="NavigationTabSelected" style="cursor: pointer" onclick="document.location.href='lichtstaerke.php';">
							Lichtstärke</td>
						</tr>
					</table>				
				</td>
			</tr>
		</table>
		
		<p>Letzte Messung: <?php echo DatabaseHandler::getLastIlluminance(); ?></p>
		
		<p><img src="lichtstaerke_graph.php" alt="[Die letzten Lichtstärken-Messungen]"></p>
	</body>
</html>
