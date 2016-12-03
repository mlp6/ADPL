from sseclient import SSEClient

if __name__ == "__main__":
    messages=SSEClient('https://api.particle.io/v1/devices/events?access_token=7883544edea996822936af401fad4209c2ba5627')

    for item in messages.__iter__():
        print(item, flush=True)
