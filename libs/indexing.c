#include <indexing.h>
#include <stl/manstr.h>
#include <panic_handler.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DOCUMENT_MAX 10000

IndexingQueue* init_indexingq(void)
{
    IndexingQueue* queue = (IndexingQueue*)malloc(sizeof(IndexingQueue));
    if (!queue) CALLPANIC("memory alloc failed!");
    queue->documents = NULL;
    queue->documentSize = 0;
    return queue;
}

static int wordChecked(str* words, str word, size_t wordsCount, size_t *index)
{
    for (size_t i = 0; i < wordsCount; i++) {
        if (strcmp(words[i], word) == 0) {
            *index = i;
            return 1;
        }
    }
    return 0;
}

static void expandMemSizeStr(str** mem, size_t* cap)
{
    *cap *= 2;
    str* tmp = (str*)realloc(*mem, sizeof(str) * (*cap));
    if (!tmp) CALLPANIC("memory realloc failed!");
    *mem = tmp;
}

static void expandMemSizeU16(uint16** mem, size_t* cap)
{
    *cap *= 2;
    uint16* tmp = (uint16*)realloc(*mem, sizeof(uint16) * (*cap));
    if (!tmp) CALLPANIC("memory realloc failed!");
    *mem = tmp;
}

static IndexingValue* init_qvalue(int index, doc document)
{
    IndexingValue *result = (IndexingValue*)malloc(sizeof(IndexingValue));
    if (!result) CALLPANIC("memory alloc failed!");

    result->index = index;
    result->name = document.name;

    size_t cap = 8;
    if (document.words_size > cap) cap = document.words_size;

    result->words = (str*)malloc(sizeof(str) * cap);
    result->instances = (uint16*)malloc(sizeof(uint16) * cap);
    if (!result->words || !result->instances) CALLPANIC("memory alloc failed!");

    size_t used = 0;

    for (size_t i = 0; i < document.words_size; i++) {
        str w = document.words[i];

        size_t existing;
        if (wordChecked(result->words, w, used, &existing)) {
            result->instances[existing] += 1;
            continue;
        }

        if (used >= cap) {
            expandMemSizeStr(&result->words, &cap);
            expandMemSizeU16(&result->instances, &cap);
        }

        result->words[used] = w;
        result->instances[used] = 1;
        used++;
    }

    if (used > 0) {
        str* wtmp = (str*)realloc(result->words, sizeof(str) * used);
        uint16* itmp = (uint16*)realloc(result->instances, sizeof(uint16) * used);
        if (wtmp) result->words = wtmp;
        if (itmp) result->instances = itmp;
    }

    result->wordSize = used;
    return result;
}

void add_documents(IndexingQueue *queue, doc **docs, size_t docCount)
{
    if (!queue) return;

    queue->documents =
    (IndexingValue**)malloc(sizeof(IndexingValue*) * docCount);
    if (!queue->documents) CALLPANIC("memory alloc failed!");

    for (size_t i = 0; i < docCount; i++) {
        queue->documents[i] = init_qvalue((int)i, *docs[i]);
    }

    queue->documentSize = docCount;
}

typedef struct {
    str word;
    uint16** docs;
    str* docNames;
    uint16* instances;
    size_t size;
    size_t cap;
} WordToDocInstance;

typedef struct {
    WordToDocInstance* items;
    size_t size;
    size_t cap;
} WordToDocIndex;

static void wtd_init(WordToDocIndex* idx)
{
    idx->items = NULL;
    idx->size = 0;
    idx->cap  = 0;
}

static void wtd_free(WordToDocIndex* idx)
{
    if (!idx) return;

    for (size_t i = 0; i < idx->size; i++) {
        for (size_t j = 0; j < idx->items[i].size; j++) {
            free(idx->items[i].docs[j]);
        }
        free(idx->items[i].docs);
        free(idx->items[i].instances);
    }
    free(idx->items);

    idx->items = NULL;
    idx->size = 0;
    idx->cap = 0;
}

static size_t wtd_find(WordToDocIndex* idx, str word)
{
    for (size_t i = 0; i < idx->size; i++) {
        if (strcmp(idx->items[i].word, word) == 0) return i;
    }
    return (size_t)-1;
}

static void wtd_ensure_items_cap(WordToDocIndex* idx)
{
    if (idx->cap == 0) {
        idx->cap = 16;
        idx->items =
        (WordToDocInstance*)malloc(sizeof(WordToDocInstance) * idx->cap);
        if (!idx->items) CALLPANIC("memory alloc failed!");
        return;
    }
    if (idx->size >= idx->cap) {
        idx->cap *= 2;
        WordToDocInstance* tmp =
        (WordToDocInstance*)realloc(idx->items,
        sizeof(WordToDocInstance) * idx->cap);

        if (!tmp) CALLPANIC("memory realloc failed!");
        idx->items = tmp;
    }
}

static void wtd_entry_init(WordToDocInstance* e, str word)
{
    e->word = word;
    e->size = 0;
    e->cap  = 4;

    e->docs = (uint16**)malloc(sizeof(uint16*) * e->cap);
    e->instances = (uint16*)malloc(sizeof(uint16) * e->cap);
    e->docNames = (str*)malloc(sizeof(str) * e->cap);

    if (!e->docs || !e->instances) CALLPANIC("memory alloc failed!");
}

static void wtd_entry_ensure_cap(WordToDocInstance* e)
{
    if (e->size >= e->cap) {
        e->cap *= 2;

        uint16** dtmp = (uint16**)realloc(e->docs, sizeof(uint16*) * e->cap);
        uint16* itmp = (uint16*)realloc(e->instances, sizeof(uint16) * e->cap);

        if (!dtmp || !itmp) CALLPANIC("memory realloc failed!");

        e->docs = dtmp;
        e->instances = itmp;
    }
}

static void wtd_add(
WordToDocIndex* idx, str word, uint16 docIndex, uint16 count, str docName
)
{
    size_t pos = wtd_find(idx, word);
    if (pos == (size_t)-1) {
        wtd_ensure_items_cap(idx);
        pos = idx->size++;
        wtd_entry_init(&idx->items[pos], word);
    }

    WordToDocInstance* e = &idx->items[pos];

    for (size_t i = 0; i < e->size; i++) {
        if (*e->docs[i] == docIndex) {
            e->instances[i] = (uint16)(e->instances[i] + count);
            return;
        }
    }

    wtd_entry_ensure_cap(e);

    e->docs[e->size] = (uint16*)malloc(sizeof(uint16));
    if (!e->docs[e->size]) CALLPANIC("memory alloc failed!");

    *e->docs[e->size] = docIndex;
    e->instances[e->size] = count;
    e->docNames[e->size] = docName;
    e->size++;
}

static void build_inverted_index(IndexingQueue* queue, WordToDocIndex* out)
{
    wtd_init(out);

    for (size_t di = 0; di < queue->documentSize; di++) {
        IndexingValue* dv = queue->documents[di];

        for (size_t wi = 0; wi < dv->wordSize; wi++) {
            wtd_add(out, dv->words[wi], (uint16)dv->index,
            dv->instances[wi], dv->name);
        }
    }
}

float tf(uint16 instance, IndexingValue *value)
{
    uint16 numOfWordInDoc = instance;
    uint16 numOfWordsInDoc = 0;
    for(size_t i = 0; i < value->wordSize; i++) {
        numOfWordsInDoc += value->instances[i];
    }

    float result = ((float)numOfWordInDoc)/((float)numOfWordsInDoc);

    return result;
}

float idf(str t, IndexingQueue *queue)
{
    uint16 documentsNum = queue->documentSize;
    uint16 docNumT = 0;

    for(size_t i = 0; i < queue->documentSize; i++) {
        IndexingValue *value = queue->documents[i];

        for(size_t j = 0; j < value->wordSize; j++) {
            if(strcmp(value->words[j], t) == 0) {
                docNumT++;
            }
        }
    }

    float result = log(((float)documentsNum/(float)docNumT));

    return result;
}

float tfidf(uint16 instance, IndexingQueue *queue, IndexingValue *value, str t)
{
    return tf(instance, value) * idf(t, queue);
}

size_t iQFindDoc(IndexingQueue *queue, str docName)
{
    for(size_t i = 0; i < queue->documentSize; i++) {
        if(strcmp((queue->documents[i]->name), docName) == 0) {
            return i;
        }
    }

    return (size_t)-1;
}

void indexing_print(IndexingQueue *queue)
{
    if (!queue || !queue->documents) return;

    WordToDocIndex idx;
    build_inverted_index(queue, &idx);

    for (size_t i = 0; i < idx.size; i++) {
        WordToDocInstance* e = &idx.items[i];

        printf("%s ", e->word);
        for (size_t j = 0; j < e->size; j++) {
            IndexingValue *value =
            (queue->documents[iQFindDoc(queue, e->docNames[j])]);


            printf("d%u/%u/%.2f ",
            (unsigned)(*e->docs[j]+1),
            (unsigned)e->instances[j],
            tfidf(e->instances[j], queue, value, e->word)
            );
        }
        printf("\n");
    }

    wtd_free(&idx);
}

void free_indexingv(IndexingValue *value)
{
    if (!value) return;
    free(value->words);
    free(value->instances);
    free(value);
}

void free_indexingq(IndexingQueue *queue)
{
    if (!queue) return;

    if (queue->documents) {
        for (size_t i = 0; i < queue->documentSize; i++) {
            free_indexingv(queue->documents[i]);
        }
        free(queue->documents);
    }

    free(queue);
}
