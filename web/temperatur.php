<?php
	/*
	 * LICENSE
	 *
	 * Copyright (c) 2014, Andreas Hasler (ah), andreas.hasler@students.ffhs.ch
	 *
	 * temperatur.php is free software copyrighted by Andreas Hasler.
	 *
	 * This program is free software: you can redistribute it and/or modify
	 * it under the terms of the GNU General Public License as published by
	 * the Free Software Foundation, either version 3 of the License, or
	 * any later version.
	 *
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
	 *
	 * This program comes with ABSOLUTELY NO WARRANTY.
	 * This is free software, and you are welcome to redistribute it
	 * under certain conditions.
	 */

	// Die Klasse für den Datenbank-Zugriff wird benötigt
	require('classes/databasehandler.php');
?>

<html>
	<head>
		<meta content="text/html; charset=utf-8" http-equiv="Content-Type">
		<link rel="stylesheet" type="text/css" href="site.css">
		<title>Raspberry Pi - Wetterstation - Temperatur</title>
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
							<td class="NavigationTabSelected" style="cursor: pointer" onclick="document.location.href='temperatur.php';">
							Temperatur</td>
							<td style="width: 5px"></td>
							<td class="NavigationTab" style="cursor: pointer" onclick="document.location.href='feuchtigkeit.php';">
							Luftfeuchtigkeit</td>
							<td style="width: 5px"></td>
							<td class="NavigationTab" style="cursor: pointer" onclick="document.location.href='druck.php';">
							Luftdruck</td>
							<td style="width: 5px"></td>
							<td class="NavigationTab" style="cursor: pointer" onclick="document.location.href='lichtstaerke.php';">
							Lichtstärke</td>
						</tr>
					</table>				
				</td>
			</tr>
		</table>
		 
		<p>Letzte Messung: <?php echo DatabaseHandler::getLastTemperature(); ?></p>
		
		<h3>Die letzten 10 Messwerte</h3>
		
		<p><img src="temperatur_graph.php" alt="[Keine Messwerte - Diagramm kann nicht dargestellt werden]"></p>
	</body>
</html>
