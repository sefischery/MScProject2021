import signal


# https://stackoverflow.com/questions/45199817/stopping-a-python-process-so-that-context-managers-still-call-exit-in-windo/45220708#45220708
class ExitClass:
    exit_now = False

    def __init__(self):
        signal.signal(signal.SIGINT, self.exit)
        signal.signal(signal.SIGTERM, self.exit)

    def exit(self, signum, frame):
        self.exit_now = True
