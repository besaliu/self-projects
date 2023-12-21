#ben liu
class RingBuffer: 
  def __init__(self, k: int): 
    self.max = k
    self.queue = [] 
     
  def en_queue(self, value: int) -> None:
    #Adds an item at the rear of the queue. 
    #If the queue is full, then remove the front element and place the new value at the back of the queue (sliding window queue)
    if len(self.queue) == self.max: #queue is full: 
      self.queue.pop(0)
      self.queue.append(value)
    else: 
      self.queue.append(value)

  def de_queue(self) -> bool:
    #Deletes the item at the front of the queue otherwise returns False if the operation is not successful. 
    if len(self.queue) == 0: #queue empty: 
      return False 
    else: 
      self.queue.pop(0) 

  def get_front(self) -> int:
    #Returns the front item from the queue. Returns -1 if the queue is empty. 
    if len(self.queue) == 0: #queue empty: 
      return -1 
    else: 
      return self.queue[0] #queue[-1] 
    
  def get_rear(self) -> int:
    #Returns the last item from the queue or -1 if the queue is empty.
    if len(self.queue) == 0: #queue empty: 
      return -1 
    else: 
      return self.queue[-1] 
    
  def is_empty(self) -> bool:
    #Returns true if the queue is empty, or false otherwise.
    if len(self.queue) == 0: #queue is empty: 
      return True 
    else: 
      return False 
    
    #return len(self.queue) == 0 

  def is_full(self) -> bool:
    #Returns true if the queue is full, or false otherwise.
    if len(self.queue) == self.max: #queue is full: 
      return True 
    else: 
      return False

  def get_average(self) -> float: 
    #Returns average of numbers in the ringBuffer, if it is empty returns None
    if len(self.queue) == 0: #empty: 
      return None 
    else: 
      sum = 0 
      counter = 0 
      for i in self.queue: 
        sum += i 
        counter += 1 
      final = sum/counter
      return (final) 

ringBuffer = RingBuffer(3)

ringBuffer.is_empty() # return True
