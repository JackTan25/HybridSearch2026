module;

export module lemmatizer;

import stl;
import status;

namespace hybridsearch {

export class Lemmatizer {
public:
    Lemmatizer(const String &path);

    ~Lemmatizer();

    Status Load();

    String Lemmatize(const String &form, const String &pos = "");

private:
    Status LoadLemmaPosOffsetMap();

    void LoadExceptionMap();

    Vector<String> Morphy(const String &form, const String &pos, bool check_exceptions = true);

    Vector<String> ApplyRules(const Vector<String> &forms, const String &pos);

    Vector<String> FilterForms(const Vector<String> &forms, const String &pos);

    String path_;

    HashMap<String, HashMap<String, Vector<int>>> lemma_pos_offset_map_;
    HashMap<String, HashMap<String, Vector<String>>> exception_map_;
    HashMap<String, int> pos_numbers_;
    HashMap<int, String> pos_names_;
    HashMap<String, String> file_map_;
    HashMap<String, Vector<Pair<String, String>>> MORPHOLOGICAL_SUBSTITUTIONS;
    Vector<String> POS_LIST;
};

} // namespace hybridsearch