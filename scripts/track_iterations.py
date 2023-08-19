# Script to explore how useful fast tracks of different sizes are.


def iterations(n_elements: int, tracks: list):
    """Use like iterations(42, [16, 1]) to see how many recursive iterations would be needed with those fast tracks."""
    tracks = sorted(tracks)
    iters = 0

    while len(tracks) > 0:
        track = tracks.pop()
        steps = n_elements // track
        iters += steps
        n_elements = n_elements % track
        print(f"{track=}, total {iters=}, current {steps=}, remaining {n_elements=}")

    if n_elements > 0:
        raise ValueError("Track failed")
    return iters
