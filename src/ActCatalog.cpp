#include "ActCatalog.h"
#include <iostream>
using namespace std;


ActCatalog::ActCatalog() {
    // ── 8 actions minimum requises, dont 2 à impact négatif ──────────────────
    // Format : ActAction(id, "texte affiché", impactMercy)
    actions.emplace(JOKE,
        ActAction(JOKE,
            "Vous racontez une blague sur les sables mouvants... le monstre rigole nerveusement.",
            +20));

    actions.emplace(COMPLIMENT,
        ActAction(COMPLIMENT,
            "Vous dites au monstre qu'il a de très beaux yeux. Il rougit (autant qu'un monstre peut rougir).",
            +25));

    actions.emplace(INSULT,
        ActAction(INSULT,
            "Vous insultez l'ancêtre du monstre. L'ambiance se détériore fortement.",
            -30));   // impact NÉGATIF

    actions.emplace(DISCUSS,
        ActAction(DISCUSS,
            "Vous engagez une discussion philosophique. Le monstre semble intrigué.",
            +15));

    actions.emplace(OBSERVE,
        ActAction(OBSERVE,
            "Vous l'observez en silence. Le monstre se sent curieusement compris.",
            +10));

    actions.emplace(PET,
        ActAction(PET,
            "Vous essayez de caresser le monstre. Il se laisse faire, un peu surpris.",
            +30));

    actions.emplace(OFFER_SNACK,
        ActAction(OFFER_SNACK,
            "Vous lui tendez un snack. Le monstre hésite... puis croque dedans.",
            +20));

    actions.emplace(REASON,
        ActAction(REASON,
            "Vous lui expliquez calmement pourquoi la violence ne résout rien.",
            +15));

    actions.emplace(DANCE,
        ActAction(DANCE,
            "Vous tentez un moonwalk. Le monstre vous regarde, bouche bée.",
            +10));

    actions.emplace(TAUNT,
        ActAction(TAUNT,
            "Vous faites des grimaces. Le monstre se met en colère.",
            -20));   // impact NÉGATIF
            
    actions.emplace(CHANT,
        ActAction(CHANT,
            "Vous entonnez une chanson. Le monstre marque la mesure du pied malgré lui.",
            +20));

    actions.emplace(BRAG,
        ActAction(BRAG,
            "Vous vous vantez de vos exploits. Le monstre lève les yeux au ciel... mais est impressionné.",
            +10));

    actions.emplace(APOLOGIZE,
        ActAction(APOLOGIZE,
            "Vous présentez vos excuses sincèrement. Le monstre semble déstabilisé.",
            +25));
}

const ActAction* ActCatalog::getAction(int id) const {
    auto it = actions.find(id);
    if (it == actions.end()) return nullptr;
    return &it->second;
}

bool ActCatalog::exists(int id) const {
    return actions.count(id) > 0;
}

void ActCatalog::displayAll() const {
    cout << "=== Catalogue ACT ===\n";
    for (const auto& [id, action] : actions) {
        cout << "[" << id << "] " << action.getDisplayText()
                  << "  (Mercy " << (action.getMercyImpact() >= 0 ? "+" : "")
                  << action.getMercyImpact() << ")\n";
    }
}
