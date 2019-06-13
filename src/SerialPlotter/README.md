## Serial Plotter

Plots an incomming signal from a Serial Port.

Written by Zuzzil

### Required Format / Protocoll information

The packages has to be send as 2 byte integers. In a set intervall 2 bytes with the value hex FC has to be send

Example:

Data Data | Data Data | FC FC | Data Data | Data Data | FC FC |