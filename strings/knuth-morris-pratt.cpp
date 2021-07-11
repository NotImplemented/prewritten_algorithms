vector<int> prefix(const std::string& sample) 
{ 
    vector<int> result(sample.size() + 1);

    int i = 0;
    int j = result[0] = -1;
    while (i < sample.size()) 
    {
        while (j > -1 && sample[i] != sample[j])
            j = result[j];
        i++;
        j++;
  
        if (sample[i] == sample[j])
            result[i] = result[j];
        else
            result[i] = j;
    }

    return result;
}
