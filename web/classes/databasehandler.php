<?php
	/*
	 * LICENSE
	 *
	 * Copyright (c) 2014, Andreas Hasler (ah), andreas.hasler@students.ffhs.ch
	 *
	 * databasehandler.php is free software copyrighted by Andreas Hasler.
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

	class DatabaseHandler
	{
		// Die letzte Temperatur ermitteln
		public static function getLastTemperature()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0 && $row['id'] > 0)
			{
				$message = round($row['temperature'], 2) . " [°C] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}
		
		// Die letzte Lichtstärke ermitteln
		public static function getLastIlluminance()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0 && $row['id'] > 0)
			{
				$message = round($row['illuminance'], 2) . " [lx] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}

		// Die letzte Luftfeuchtigkeit ermitteln
		public static function getLastHumidity()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0 && $row['id'] > 0)
			{
				$message = round($row['humidity'], 2) . " [%] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}

		// Der letzte Luftdruck ermitteln
		public static function getLastPressure()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0 && $row['id'] > 0)
			{
				$message = round($row['pressure'], 2) . " [hPa] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}
	}
?>

<html>
<head>
<meta content="text/html; charset=utf-8" http-equiv="Content-Type">
</head>
<body></body>
</html>
