Stephanie Lin CruzID slin41@ucsc.edu Partner: Anirudh Sridhar

MY REPO CONTAINS THE FINAL VERSION OF THE PROJECT.

I wrote:
Negotiation.C
Message.C
AgentTest.C
FieldTest.C

Our modules should work individually but when put together, they don't seem to work.

Introduction:
	In this lab, our requirement was to build a game similar to Battleship. The players are suppose to interact with their own I/O shields with their boards connected to each others. We used two state machines to implement this lab, one FSM and one HSM, which is much more complicated than our other labs. The game starts with a button press from player 1. This sends a challenging request to player 2. Player 2 generates a random number and sends it to player 1, generating a acceptance received. The negotiation generates a random number and a hash based on a key. This is sent to the other player to determine HEADS AND TAILS of which player goes first. However to make the game fair, player two verifies if player 1 is cheating.
	During the gameplay, each player takes turns attacking and defending. While player 1 is attacking, they will send a message to player 2 so that the defending player 2 will update their own field and then let player 1 if they hit or miss. When it is player's 2 turn to attack, it is player's 1 turn to defend and so one. Once a player sinks all the boats of the other player, they will receive a victory screen while the other player will receive defeat message. Then players can reset the game. To make sure all the messages are transmitted safely,  we use message_decode and message_encode to send and receive messages.
	Anirudh worked on Field.C and Agent.C while I worked on Negotiation.C and Message.C. Then we wrote each other's test harnesses. We wrote our own functions separately, but worked together to debug our errors. In negotiation, NegotiationHash uses Hash(A) = A^(2)mod 0xBEEF formula to generate the hash. NegotiationVerify takes the secret and hash and compares they are the same. In NegotiateCoinFlip, I take the bit parity of both random generated numbers to calculate if the result is heads or tails. In Message.C, I calculate the checksum by taking the payload, and xoring each character. In Message_ParseMessage, I called CalculateChecksum to calculate the check sum using the payload and then compare it with the checksum in the message to if they work. In Message_Encode, I wrap each payload template with a message template. In Message_Decode, I implemented a state machine to record the payload, checksum, and then generate the correct message event.
	Our individual modules worked except for Agent.C. For some reason it would constantly error out even though though the state machine was really simple. Everything else seemed to work well. I found making test harnesses really helpful in addition to using print statements. It keeps everything organized.
	This lab was really time consuming but I learned to debug more efficiently. It reinforced my understanding of statemachines and pointers too. I enjoyed the theme of this lab, Battleships. It inspires me to make my own game as well. This lab is fine, but I think the .o files might not be working for everyone.

	
