# Airport Online

# Introduction 
The objective of the project is to develop software that allows to manage passengers in an airport.
The software manages the check-in and boarding of flights. As a result, passengers must have purchased a ticket for the flight to be taken.
A passenger’s journey is as follows: the passenger registers and drop off his luggage. After the deposit of the luggage, he crosses the borders, security and he gets on board.
Two types of passengers are taken into account: passengers without privilege and "priority" passengers.

# Features 
- Add a passenger on a flight (this step is to simulate the purchase of the ticket). This functionality generates a ticket number associated with passenger information (name, first name, date of birth, passport number, destination, etc.). The ticket with all poster information.

- Register a passenger by providing his name or ticket number. Boarding pass with all the information (Ticket number, last name, first name, number of baggage, seat number, privilege, etc.) is displayed.
  - Passenger check-in generates a boarding pass and one or more tickets luggage if the passenger has it.
  - Priority passengers can drop off two pieces of baggage while passengers without privilege can only file one maximum.
  - The luggage of a "priority" passenger must also be marked "priority".
  - The "boarding pass" includes a seat which is chosen automatically (randomly) or at the request of the passenger and according to the seats available on the plane.
  
- Pass the passenger through the border post and check that they meet all the conditions to reach its destination. At the end of this step, the program displays the nationality of the passenger, his destination and whether he needs a visa.

- Pass the passenger through the security checkpoint (the passenger must comply with all the conditions for hand luggage on liquids and items to be taken in the cabin). The program displays the products to be collected for a passenger.

- Embark the passengers on the plane. "VIP" passengers have priority and must all come before non-privileged passengers. Therefore, if a "VIP" passenger has not boarded, all other passengers without privilege cannot. The program displays the filling status of the aircraft at the end of boarding and the passengers who did not board.

- Load one or more pieces of luggage on the plane.

- Check that the aircraft can take off by checking that all registered passengers have boarded and that all baggage is loaded.

- Consult flight statistics (current flights, aircraft fill rate, passenger and VIP rate).

