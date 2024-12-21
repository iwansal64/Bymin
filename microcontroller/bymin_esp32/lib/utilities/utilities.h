void rightShiftArray(byte arr[], int size)
{
    // Shift each element to the right by one position
    for (int i = size - 1; i > 0; i--)
    {
        arr[i] = arr[i - 1];
    }
    // Fill the first element with a default value (e.g., 0)
    arr[0] = 0;
}
