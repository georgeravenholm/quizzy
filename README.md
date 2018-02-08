# quizzy
quiz game from text file

file format sgit

``quiz:`` and ``quizend:`` must be at start and end of file

``question:`` start of question, line underneath is question text

``correct:`` and ``wrong:`` answers to questions, as many as you wish. line underneath is the text

``questionend:`` must be at end of question text and answers

``# this is a comment``

example
```
quiz:
# im a comment dude

question:
whats this repo called
  correct:
    quizzy
  wrong:
    sexe
  wrong:
    meme
  wrong:
    pain in the caner
questionend:

quizend:
```
