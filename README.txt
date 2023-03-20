This project originally took a dictionary file (vocabulary.txt) and read it into a dictionary tree. It then took another file (testfile1.txt), in this case a book, and checked to see how many of the words in the book were prefixes to the words in the vocabulary file. For example, the book may have the word "Prejudice", which is a prefix for the words "prejudice, prejudice's, prejudiced, prejudices", and therefore returns a count of 4 for the word Prejudice.

In the second iteration of the code, I used multithreading to run these tasks seperately. 

The main driver starts the populateree thread as well as the populatetree progress bar. As the populatetree begins to load the dictionary tree with vocabulary words, the populateree progress bar keeps count of how many words are currently being read in, and prints a time accurate bar while loading. Then, once the populate tree thread is done, I begin both the readprefix and countprefix threads at the same time. 

The readprefix counts the words read in and then uses a thread lock to be able to push the words onto a queue. The countprefix takes these words one at a time and checks to see how many times it is a prefix in the vocabulary file, then locks the queue to remove the word. It then also pushes the word along with this count to "countprefix_output.txt". Once the readprefix thread is finished and we have an official word count, we launch the countprefix progress bar while countprefix continues running which will then give us a time accurate representation of how many prefixes have been processed. The thread locking is important because without it a race condition is created in the queue.

TO RUN:

make
./countprefix vocabulary.txt testfile1.txt

OR

./countprefix vocabulary.txt testfile1.txt -p 40 -h 5 -n 3

-p will set how many marks you want total for the progress bar, -h will set the mark interval, and -n will set the prefix count minimum to be uploaded to the output file
