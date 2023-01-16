# https://techdevguide.withgoogle.com/resources/former-interview-question-flatten-iterators/


def interleave(*iterables):
    n = len(iterables)
    iterators = [iter(k) for k in iterables]
    current  = 0
    finished = 0
    while True:
        try:
            yield next(iterators[current])
            finished = 0
        except StopIteration:
            finished += 1
            if finished == n:
                return None
        finally:
            current = (current + 1) % n


a = ["hi", "bye", "one", "two"]
b = ['a', 'b', 'c', 'd', 'e', 'f', 'g']
c = [1, 2, 3, 4, 5]

x = interleave(a, b, c)
for j in x:
    print(j)
