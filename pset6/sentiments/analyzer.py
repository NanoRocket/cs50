#  imports the Natural Language Toolkit, among whose features is a tokenizer
#  that you can use to split a tweet (which is maximally a 140-character str 
#  object) into a list of words (i.e., shorter str objects).

import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer.
        positives, whose value is the path to a text file containing positive words; 
        and negatives, whose value is the path to a text file containing negative words.
        """
        # I add words to a large set for efficiency, stripping out white space and /n
        
        # get set of positive word
        file = open('positive-words.txt')
        self.positives = []
        for text in file:
            if text.startswith(';'):
                continue
            else:
                self.positives.append(text.strip())
        self.positives = set(self.positives)
        file.close()
        
        # get set of negative words
        file = open('negative-words.txt')        
        self.negatives = []
        for text in file:
            if text.startswith(';'):
                continue
            else:
                self.negatives.append(text.strip())
        self.negatives = set(self.negatives)
        file.close()

    def analyze(self, text):
        """A str to be analyzed for sentiment. , returning its score.
        analyze analyzes the sentiment of text, returning a positive score if text 
        is more positive than negative, a negative score if text is more negative 
        than positive, and 0 otherwise
        """
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        word_score = 0
        
        for word in tokens:
            if word.lower() in self.positives:
                word_score += 1
            if word.lower() in self.negatives:
                word_score += -1
            else:
                word_score += 0
        
        return word_score
