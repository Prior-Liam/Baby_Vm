# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    README.txt                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: liamprior <liamprior@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/01 14:30:48 by liamprior         #+#    #+#              #
#    Updated: 2022/11/30 23:37:35 by liamprior        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

The program is simple to use:
1.) run make.
2.) type "./pa1 {inputfile}" with {inputfile} being the arguemnt file name.



***FIRST turn in*****
The flow of program:
First it opens and reads the file
After that i get the file size and guarentee that my buffer can hold enough space.
i store each line of the file into a string where it is parsed until i find the first 0x
after i find the first 0x, i move my pointer and store the following hex string into a temp string.
This temp string is then converted into a decimal equivelent.
This is repeated for the next hex value given.
The decimal equivelents are added together for a total.
While this is happening i check for overflows by seeing if the added total is less than either of the numbers given.
With the total in a decimal form, i convert it into a hex string with a loop in the program.
This string is backwards so when i print, i have to start from then end and decrement.
Thats the general overview of whats going on, sorry its all in the main but im very busy and ill break it up later for the next program.


***UPDATE project 2***
The flow is the exact same, I simply modularized my code and added more operations. To add the operations, as i parse the file i look for
the key phrases the represent which operation to do and after that i do the operation. I grouped ORR XOR ADD SUB AND together becuase they
all take two hex arguments to keep the code short. I did the same thing for the logical shifters because they take a hex value then a dec 
value after.
Not is in its on catagory because it only takes one hex value.

The problem with my code right now is that i have a memory leak, i could fix but it would require that i dont loose refrence to my alloced memory
and that would mean i need to go through my code and leaks arnt on the criteria so i will leave for now.

Also i have a functional overflow flag but i removed because it too is not required for criteria.

***UPDATE project 3***

I use an int array for the registers and the list index is where i store imm values. I access those registers by storing their indexes into
seperate variables i have in the struct. That info is passed onto where i complete opps and is stored appropriately. 
For asr i simply just use a cast so the shifting operator treats the first bit with some respect! Lots of code but its pretty straight forward.