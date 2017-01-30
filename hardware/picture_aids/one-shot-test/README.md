Test the output of the one-shot to help figure out why we might be missing
bucket tip events.

O-scope screen captures of the one-shot output after a simulated bucket tip
event are shown in the images in this directory.

Note that:
* The output pulse is ~2 s in duration.
* The voltage during the rect output is relatively stable.

It turns out that the issues w/ the missed events were due to the slow exection
of ``loop()`` b/c of the 5 temp probe reads.  This was addressed in spefcific
software modifications.
