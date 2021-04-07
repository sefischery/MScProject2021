FROM python:3.9.2-slim-buster

# Define the working directory in the container
WORKDIR /app

# Copy and install the requirements.
COPY requirements.txt /app/requirements.txt

#Install requirements defined by virtual env
RUN pip3 install -r requirements.txt

#COPY entire source
COPY . .

CMD ["python3", "UDP_Python_Server.py"]
