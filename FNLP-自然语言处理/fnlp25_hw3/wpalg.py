from transformers import AutoTokenizer
from collections import defaultdict


def wordpiece(training_corpus, vocab_size):
    tokenizer = AutoTokenizer.from_pretrained("bert-base-uncased")

    word_freqs = defaultdict(int)
    for text in training_corpus:
        words_with_offsets = tokenizer.backend_tokenizer.pre_tokenizer.pre_tokenize_str(text)
        new_words = [word for word, offset in words_with_offsets]
        for word in new_words:
            word_freqs[word] += 1

    alphabet = []
    for word in word_freqs.keys():
        if word[0] not in alphabet:
            alphabet.append(word[0])
        for letter in word[1:]:
            if f"##{letter}" not in alphabet:
                alphabet.append(f"##{letter}")

    alphabet.sort()

    vocab = ["[PAD]", "[UNK]", "[CLS]", "[SEP]", "[MASK]"] + alphabet.copy()

    # Do NOT add your above this line.
    #======
    
    # Add your code here.

    #======
    # Do NOT add your below this line.

    return vocab

if __name__ == "__main__":
    default_training_corpus = [
        "peking university is located in haidian district",
        "computer science is the flagship major of peking university",
        "the school of electronic engineering and computer science enrolls approximately five hundred new students each year"  
    ]

    default_vocab_size = 120

    my_vocab = wordpiece(default_training_corpus, default_vocab_size)

    print('The vocab:', my_vocab)

    def encode_word(custom_vocab, word):
        tokens = []
        while len(word) > 0:
            i = len(word)
            while i > 0 and word[:i] not in custom_vocab:
                i -= 1
            if i == 0:
                return ["[UNK]"]
            tokens.append(word[:i])
            word = word[i:]
            if len(word) > 0:
                word = f"##{word}"
        return tokens

    def tokenize(custom_vocab, text):
        tokenizer = AutoTokenizer.from_pretrained("bert-base-uncased")
        pre_tokenize_result = tokenizer._tokenizer.pre_tokenizer.pre_tokenize_str(text)
        pre_tokenized_text = [word for word, offset in pre_tokenize_result]
        encoded_words = [encode_word(custom_vocab, word) for word in pre_tokenized_text]
        return sum(encoded_words, [])

    print('Tokenization result:', tokenize(my_vocab, 'nous etudions a l universite de pekin'))
