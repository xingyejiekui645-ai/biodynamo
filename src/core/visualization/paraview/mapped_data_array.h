template <typename TScalar, typename TClass, typename TDataMember>
vtkArrayIterator* MappedDataArray<TScalar, TClass, TDataMember>::NewIterator() {
  vtkErrorMacro(<< "Not implemented.");
  return nullptr;
}
