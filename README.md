# General Power Theory (MLT)
General Power Theory (GPT) algorithm implementation for calculation of minimal loss current CRMS vector to improve efficiency of power delivery systems.
<br>
<br>

## Project Structure
<b>gpt_algorithm_python/</b> Implementation of GPT algorithm in Jupyter Notebook using Python<br>
<b>powerstar_gpt/</b><br>
|---python/ Python software for capturing and processing data using Powerstar 10 board<br>
    |---powerstar_gpt.ipynb: Jupyter Notebook file to read from Powerstar 10 card and perform GPT algorithm on measured data. Useful for development and testing.<br>
    |---powerstar_read.py: Python script to read from Powerstar 10 card. *Incomplete* <br>
    |---powerstar_close_relays.py: Python script to close relays on board and allow a load to be connected. *Incomplete* <br>
    |---requirments.txt: Python package requirements.
