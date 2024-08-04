const express = require('express');
const bodyParser = require('body-parser');
const path = require('path');

const app = express();
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Serve static files from the "public" directory
app.use(express.static(path.join(__dirname, 'public')));

class TypingGame {
    constructor() {
        this.words = ["word1", "word2", "word3", "word4", "word5", "word6"];
        this.index = 5;
        this.players = {};
    }

    addPlayer(username, typingSpeed = 0.0, accuracy = 0.0) {
        this.players[username] = { typingSpeed, accuracy };
    }

    checkWord(typedWord, currentWord) {
        return typedWord === currentWord;
    }

    getNextWord() {
        if (this.index < this.words.length) {
            return this.words[this.index++];
        }
        return "";
    }

    getInitialWords() {
        return this.words.slice(0, 5);
    }
}

const game = new TypingGame();

app.post('/start-game', (req, res) => {
    const username = req.body.username;
    game.addPlayer(username);
    res.redirect('/gamepage.html');
});

app.post('/start-typing', (req, res) => {
    res.json({ words: game.getInitialWords() });
});

app.post('/submit-word', (req, res) => {
    const { typedWord, currentWord } = req.body;
    const isCorrect = game.checkWord(typedWord, currentWord);
    const response = { isCorrect };
    if (isCorrect) {
        response.nextWord = game.getNextWord();
    }
    res.json(response);
});

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => {
    console.log(`Server started on http://localhost:${PORT}`);
});
